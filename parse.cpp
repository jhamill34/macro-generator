#include <iostream>
#include <stack>
#include <queue>
#include <exception>
#include "tokens.h"
#include "composite.h"
#include "parse.h"

using namespace std;

// TODO: create an error function 

//======== PUBLIC METHODS ============
/**
 * Constructor 
 * @param queue<Token *> * _token_queue 
 */
Parse::Parse(queue<Token *> * _token_queue){
  token_queue = _token_queue;
}

/**
 * Initial call that returns a composite object
 * PROGRAM := STATEMENT_LIST
 * @return Composite * _root
 */
Composite * Parse::parseProgram(){
  // Create a root node
  Token * program = new Token();
  program->value = "Program Start";
  program->type = PROGRAM;
  Composite * _root = new Composite(program);

  // Start pulling out tokens
  while(!token_queue->empty() && token_queue->front()->type != EOP){
    // We expect statements so we call the private method
    _root->addChild(parseStatement());
  }

  return _root;
}


//======== PRIVTATE METHODS ==========
/**
 * From the current state of the queue looks for 
 * tokens pertaining to a "statement"
 * STATEMENT := ASSIGNMENT ; | LOOP_BLOCK | CONDITIONAL_BLOCK 
 * @return Composite * _statement
 */
AbstractComposite * Parse::parseStatement(){
    AbstractComposite * _statement;
    Token * next = token_queue->front();

    if(next->type == FOR_LOOP_STATEMENT){
      _statement = new Composite(next);
      token_queue->pop();
      _statement = parseLoopBlock((Composite *)_statement);

    }else if(next->type == IF_CONDITION){
      _statement = new Composite(next);
      token_queue->pop();
      _statement = parseConditionalBlock((Composite *)_statement);

    }else if(next->type == IDENTIFIER 
          || (next->type == SYMBOL && next->value.size() == 2 && ((next->value[0] == MINUS && next->value[1] == MINUS) 
                                     || (next->value[0] == PLUS && next->value[1] == PLUS)))){
      _statement = parseAssignment();
      if(token_queue->front()->type != END_STATEMENT){
        cout << "Syntax Error: mising semi-colon" << endl;
        exit(1);
      }
    }else if(next->type == INLINE_START){
      _statement = parseInline();
    }else if(next->type == PRINT){
      _statement = new Leaf(next);
    }else{
      cout << "Syntax Error: expecting valid statement: " << next->value << endl;
      exit(1); 
    }

    // remove the next token
    token_queue->pop();
    return _statement;
}

/**
 * Looks for tokens or Identifiers to insert using 
 * the inline tags
 * INLINE := INLINE_OPEN IDENTIFIER INLINE_CLOSE
 * @return [description]
 */
Composite * Parse::parseInline(){
  Token * next = token_queue->front();
  Composite * _inline = new Composite(next);
  token_queue->pop();

  next = token_queue->front();
  while(next->type != INLINE_END && next->type != EOP){
    Leaf * _sub = new Leaf(next);
    if(next->type == IDENTIFIER){
      _inline->addChild(_sub);  
    }else{
      cout << "Syntax Error: expected variable inside inline tokens" << endl;
      cout << next->value << endl;
      exit(1);
    }
    token_queue->pop();
    next = token_queue->front();
  }

  if(next->type == EOP){
    cout << "Syntax Error: Reached end of file before finding Closing Inline" << endl;
    exit(1);
  }

  return _inline;
}

/**
 * Looks for tokens pertaining to an "assignment"
 * ASSIGNMENT := INDENTIFIER SYMBOL EXPRESSION
 * @return Composite * _assignment
 */
Composite * Parse::parseAssignment(){
  Token * next = token_queue->front();
  Composite * _statement;

  // Create leaf object
  Leaf * _identifier = new Leaf(next);

  // Pop and look to see if we have an '='
  token_queue->pop(); 
  Token * sym = token_queue->front();
  if(sym->type == SYMBOL && sym->value.size() == 1 && sym->value[0] == EQ){
    _statement = new Composite(sym);
    token_queue->pop();

    // Add children
    _statement->addChild(_identifier);
    _statement->addChild(parseExpression());
  }else if(_identifier->getData()->type == IDENTIFIER 
          && sym->value.size() == 2 && sym->value[1] == EQ 
          && (sym->value[0] != LT || sym->value[0] != GT || sym->value[0] != EQ || sym->value[0] != BANG)){
    // Break the current symbol into two
    Token * assign = new Token();
    assign->type = SYMBOL;
    assign->value = EQ;

    Token * op = new Token();
    op->type = SYMBOL;
    op->value = sym->value[0];
    if(op->value[0] == PLUS || op->value[0] == MINUS){
      op->priority = ADD_SUB;
    }else if(op->value[0] == ASTERISK || op->value[0] == FSLASH){
      op->priority = MULT_DIV;
    }else if(op->value[0] == CARET){
      op->priority = EXP;
    }else{
      op->priority = DEFAULT_PRIORTY;
    }
    // Create a copy of the identifier token
    Token * identifier_copy = new Token();
    identifier_copy->type = IDENTIFIER;
    identifier_copy->value = _identifier->getData()->value;

    Leaf * left = new Leaf(identifier_copy);
    Composite * op_new = new Composite(op);
    op_new->addChild(left);
    token_queue->pop();
    op_new->addChild(parseExpression());

    _statement = new Composite(assign);
    _statement->addChild(_identifier);
    _statement->addChild(op_new);
  }else{
    cout << "Syntax Error: expected assignment operator" << endl;
    cout << sym->value << endl;
    exit(1);
  }

  return _statement;
}

AbstractComposite * Parse::parseExpression(){
  return parseExpressionHelp(0);
}

/**
 * Looks for tokens pertaining to an "expression"
 * recursive call until you don't see a constant or identifier
 * EXPRESSION := [IDENTIFIER|CONSTANT] SYMBOL [IDENTIFIER|CONSTANT]
 * @return Composite * _expression
 */
AbstractComposite * Parse::parseExpressionHelp(unsigned int paren_depth){
  Token * next = token_queue->front();
  Composite * _expression;


  // If we see an open parenthesis then increment the counter
  while(next->type == OPEN_PAREN){
    // Push onto the stack to ensure the correct nesting
    nesting_stack.push(next->type); 
    paren_depth += PAREN_DELTA;
    token_queue->pop();
    next = token_queue->front();
  }

  if(next->type == IDENTIFIER || IS_CONSTANT(next->type)){
    Leaf * left_side = new Leaf(next);
    token_queue->pop();
    next = token_queue->front();

    while(next->type == CLOSE_PAREN && nesting_stack.top() == OPEN_PAREN){
      nesting_stack.pop();
      paren_depth -= PAREN_DELTA;
      token_queue->pop();
      next = token_queue->front();
    }

    if(next->type == SYMBOL){
      if(left_side->getData()->type == IDENTIFIER 
          && next->value.size() == 2 && next->value[1] == EQ 
          && (next->value[0] != LT && next->value[0] != GT && next->value[0] != EQ && next->value[0] != BANG)){
        // Break the current symbol into two
        Token * assign = new Token();
        assign->type = SYMBOL;
        assign->value = EQ;

        Token * op = new Token();
        op->type = SYMBOL;
        op->value = next->value[0];
        if(op->value[0] == PLUS || op->value[0] == MINUS){
          op->priority = ADD_SUB;
        }else if(op->value[0] == ASTERISK || op->value[0] == FSLASH){
          op->priority = MULT_DIV;
        }else if(op->value[0] == CARET){
          op->priority = EXP;
        }else{
          op->priority = DEFAULT_PRIORTY;
        }
        op->priority += paren_depth;

        // Create a copy of the identifier token
        Token * identifier_copy = new Token();
        identifier_copy->type = IDENTIFIER;
        identifier_copy->value = left_side->getData()->value;

        Leaf * left = new Leaf(identifier_copy);
        Composite * op_new = new Composite(op);
        op_new->addChild(left);
        token_queue->pop();
        op_new->addChild(parseExpressionHelp(paren_depth));
        
        _expression = new Composite(assign);
        _expression->addChild(left_side);
        _expression->addChild(op_new);      
      }else{
        next->priority += paren_depth;
        _expression = new Composite(next);
        _expression->addChild(left_side);
        token_queue->pop();
        _expression->addChild(parseExpressionHelp(paren_depth)); // Recursive call

        // Condition to rotate
        if(_expression->getData()->priority > _expression->getChild(1)->getData()->priority){
          // Temporary pointer so we can keep track of the child
          // First Rotation
          _expression = rotateLeft(_expression);
          AbstractComposite * tmp_left = _expression->getChild(0);
          if(tmp_left->getData()->priority > tmp_left->getChild(1)->getData()->priority){
            tmp_left = rotateLeft((Composite *)tmp_left);
            _expression->setChild(tmp_left, 0);
          }
        }

      }
    }else{
      return left_side;
    }
  }else{
    cout << "Syntax Error: expected valid expressio arg " << endl;
    cout << next->value << endl;
    cout << "ROW: " << next->row_num << endl;
    cout << "COL: " << next->col_num << endl;
    exit(1); 
  }

  return _expression;
}


/**
 * Looks or tokens that look like blocks
 * BLOCK := STATEMENT_LIST
 * @return Composite * _block
 */
Composite * Parse::parseBlock(){
  Token * next = token_queue->front();
  Composite * _block;

  // parse block part of the forloop
  // look for open and close brackets
  if(next->type == OPEN_BLOCK){
    nesting_stack.push(next->type);
    _block = new Composite(next);
    token_queue->pop();
  }else{
    cout << "Syntax Error: Expected open braket" << endl;
    cout << next->value << endl;
    exit(1);
  }
  while(next->type != CLOSE_BLOCK && next->type != EOP){
    _block->addChild(parseStatement());
    next = token_queue->front();
  }

  if(next->type == EOP){
    cout << "Reached end of file before saw a closing bracket" << endl;
  }else{
    nesting_stack.pop();
    token_queue->pop();
  }
  return _block;
}

/**
 * Looks for tokens that look like a loop block
 * x = change(x) paradigm
 * LOOP_BLOCK := for ( EXPRESSION ; EXPRESSION ; EXPRESSION ) { BLOCK }
 * @return [description]
 */
Composite * Parse::parseLoopBlock(Composite * _loopblock){
  Token * next = token_queue->front();
  if(next->type == OPEN_PAREN){
    nesting_stack.push(next->type);
    token_queue->pop();
  }else{
    cout << "Syntax Error: Missing Parenthesis" << endl;
    cout << next->value << endl;
    exit(1);
  }

  // Add first expression
  _loopblock->addChild(parseExpression());

  // Check for semi-colon
  next = token_queue->front();
  if(next->type == END_STATEMENT){
    token_queue->pop();
  }else{
    cout << "Syntax Error: Missing Semi colon after expression" << endl;
    cout << next->value << endl;
    exit(1);
  }

  // Add second expression
  _loopblock->addChild(parseExpression());

  next = token_queue->front();
  if(next->type == END_STATEMENT){
    token_queue->pop();
  }else{
    cout << "Syntax Error: Missing Semi colon after expression" << endl;
    cout << next->value << endl;
    exit(1);
  }

  _loopblock->addChild(parseExpression());


  // parseBlock looks for the closing bracket
  _loopblock->addChild(parseBlock());
  
  // No need to really return it since
  // we worked on the pointer
  return _loopblock;
}

/**
 * Looks for tokens that look like a conditional 
 * x = change(x) paradigm
 * CONDITIONAL_BLOCK := if ( EXPRESSION ) { BLOCK }
 *                    | else { BLOCK }
 * @return [description]
 */
Composite * Parse::parseConditionalBlock(Composite * _conditionblock){
  Token * next = token_queue->front();
  if(next->type == OPEN_PAREN){
    nesting_stack.push(next->type);
    token_queue->pop();
  }else{
    cout << "Syntax Error: Missing Parenthesis" << endl;
    cout << next->value << endl;
    exit(1);
  }

  _conditionblock->addChild(parseExpression());


  _conditionblock->addChild(parseBlock());

  next = token_queue->front();
  if(next->type == ELSE_CONDITION){
    token_queue->pop();
    next = token_queue->front();
    if(next->type == IF_CONDITION){
      Composite * _nestedIf = new Composite(next);
      token_queue->pop();
      _nestedIf = parseConditionalBlock(_nestedIf);
      _conditionblock->addChild(_nestedIf);
    }else{
      _conditionblock->addChild(parseBlock());
    }
  }

  return _conditionblock;
}
