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

    }else if(next->type == IF_CONDITION || next->type == ELSE_CONDITION){
      _statement = new Composite(next);
      _statement = parseConditionalBlock((Composite *)_statement);

    }else if(next->type == IDENTIFIER){
      _statement = parseAssignment();
      if(token_queue->front()->type != END_STATEMENT){
        cout << "Syntax Error: mising semi-colon";
        exit(1);
      }
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
  if(sym->type == SYMBOL && sym->value[0] == EQ){
    _statement = new Composite(sym);
    token_queue->pop();

    // Add children
    _statement->addChild(_identifier);
    _statement->addChild(parseExpression());
  }else{
    cout << "Syntax Error: expected assignment operator" << endl;
    exit(1);
  }

  return _statement;
}

/**
 * Looks for tokens pertaining to an "expression"
 * recursive call until you don't see a constant or identifier
 * EXPRESSION := [IDENTIFIER|CONSTANT] SYMBOL [IDENTIFIER|CONSTANT]
 * @return Composite * _expression
 */
AbstractComposite * Parse::parseExpression(){
  Token * next = token_queue->front();
  Composite * _expression;
  if(next->type == IDENTIFIER || IS_CONSTANT(next->type)){
    Leaf * left_side = new Leaf(next);
    token_queue->pop();
    next = token_queue->front();
    if(next->type == SYMBOL){
      _expression = new Composite(next);
      _expression->addChild(left_side);
      token_queue->pop();
      _expression->addChild(parseExpression()); // Recursive call
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

  // Ensure proper nesting with closing paren
  next = token_queue->front();
  if(next->type == CLOSE_PAREN && nesting_stack.top() == OPEN_PAREN){
    nesting_stack.pop();
    token_queue->pop();
  }else{
    cout << "Syntax Error: Missing closing Parenthesis" << endl;
    cout << next->value << endl;
    token_queue->pop();
    cout << token_queue->front()->value << endl;
    exit(1);
  }


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
Composite * Parse::parseConditionalBlock(Composite *){
  return NULL;
}
