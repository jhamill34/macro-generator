#include <iostream>
#include <stack>
#include <queue>
#include "tokens.h"
#include "composite.h"
#include "parse.h"

using namespace std;


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
  while(!token_queue->empty()){
    // We expect statements so we call the private method
    parseStatement();
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
Composite * Parse::parseStatement(){
    Token * next = token_queue->front();

    cout << next->value << endl;

    // remove the next token
    token_queue->pop();
    return NULL;
}

/**
 * Looks for tokens pertaining to an "assignment"
 * ASSIGNMENT := INDENTIFIER SYMBOL EXPRESSION
 * @return Composite * _assignment
 */
Composite * Parse::parseAssignment(){
  return NULL;
}

/**
 * Looks for tokens pertaining to an "expression"
 * EXPRESSION := [IDENTIFIER|CONSTANT] SYMBOL [IDENTIFIER|CONSTANT]
 * @return Composite * _expression
 */
Composite * Parse::parseExpression(){
  return NULL;
}

/**
 * Looks for a token that is a "constant"
 * CONSTANT := NUMBER | STRING
 * @return Leaf * _constant
 */
Leaf * Parse::parseConstant(){
  return NULL;
}

/**
 * Looks or tokens that look like blocks
 * BLOCK := STATEMENT_LIST
 * @return Composite * _block
 */
Composite * Parse::parseBlock(){
  return NULL;
}

/**
 * Looks for tokens that look like a loop block
 * LOOP_BLOCK := for ( EXPRESSION ; EXPRESSION ; EXPRESSION ) { BLOCK }
 * @return [description]
 */
Composite * Parse::parseLoopBlock(){
  return NULL;
}

/**
 * Looks for tokens that look like a conditional 
 * CONDITIONAL_BLOCK := if ( EXPRESSION ) { BLOCK }
 *                    | else { BLOCK }
 * @return [description]
 */
Composite * Parse::parseConditionalBlock(){
  return NULL;
}
