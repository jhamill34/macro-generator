#include <iostream>
#include <stack>
#include <queue>
#include "tokens.h"
#include "composite.h"

using namespace std;

//
//  IDENTIFIER ASIGNMENT_OP EXPRESSION | CONDITIONAL_EXP END_STATEMENT 
//

//
//  CONDITIONAL_BLOCK | LOOP_BLOCK | ASSIGNMENT  
//

//
//  <IDENTIFIER|NUMBER|STRING> OPERATOR <IDENTIFIER|NUMBER|STRING>
//


// 
//  for ( EXPRESSION; CONDITIONAL_EXP; EXPRESSION ) { BLOCK }
//

//
// if ( CONDITIONAL_EXP ) { BLOCK } else? { BLOCK }
//


//
//  <IDENTIFIER | NUMBER | STRING> CONDITIONAL_SYMBOL <IDENTIFIER | NUMBER | STRING>
//


//
//  { STATEMENT_LIST } -> { STATEMENT  STATEMENT  ... STATEMENT  }
//

Composite * 
parseForLoop(queue<Token *> *token_queue){
  stack<TokenType> closure_stack;
  Composite * parent = new Composite(token_queue->front());
  token_queue->pop();
  
  if(token_queue->front()->type == OPEN_PAREN){
    closure_stack.push(token_queue->front()->type);
    token_queue->pop();
  }else{
    // TODO: keep track of the location so we can point to 
    // where the syntax error occured
    cout << "Syntax Error: mising parenthesis --> for (" << endl;
    exit(1);
  }
  
  // parse for loop
  // parent->addChild(parseStatement(token_queue));

  if(token_queue->front()->type != END_STATEMENT){
    cout << "Syntax Error: missing semi-colon after expression" << endl;
    exit(1);
  }else{
    token_queue->pop();
  }

  // parent->addChild(parseConditional(token_queue));

  if(token_queue->front()->type != END_STATEMENT){
    cout << "Syntax Error: missing semi-colon after expression" << endl;
    exit(1);
  }else{
    token_queue->pop();
  }

  // parent->addChild(parseStatement(token_queue));

  if(token_queue->front()->type == CLOSE_PAREN && closure_stack.top() == OPEN_PAREN){
    closure_stack.pop();
  }else{
    cout << "Syntax Error: missing parenthesis --> for ( ... )" << endl;
    exit(1);
  }

  if(token_queue->front()->type == OPEN_BLOCK){
    closure_stack.push(token_queue->front()->type);
    token_queue->pop();
  }else{
    // TODO: keep track of the location so we can point to 
    // where the syntax error occured
    cout << "Syntax Error: mising block start --> for ( ... ) {" << endl;
    exit(1);
  }

  // parent->addChild(parseBlock(token_queue));

  return parent;
  
}

Composite *
parseBlock(queue<Token *> *token_queue, Composite * parent){
  stack<TokenType> closure_stack;
  
  if(token_queue->front()->type == CLOSE_BLOCK && closure_stack.top() == OPEN_BLOCK){
    closure_stack.pop();
  }else{
    cout << "Syntax Error: missing block end --> for ( ... ) { stuff }" << endl;
    exit(1);
  }
  return NULL;
}


void 
parseTokens(queue<Token *>* token_queue){
  Token * tmp;
  
  Token * prog = new Token;
  prog->value = "root";
  prog->type = PROGRAM;

  Composite * root = new Composite(prog);

  while(!token_queue->empty()){
    tmp = token_queue->front();
    if(tmp->type == FOR_LOOP_STATEMENT){
      // root->addChild(parseForLoop(token_queue));
    }else if(tmp->type == IF_CONDITION){
      //look for condition and block 
    }else if(tmp->type == ELSE_CONDITION){
      // look for block
    }
  }
}
