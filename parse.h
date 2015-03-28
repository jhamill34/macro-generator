#ifndef _PARSE_H_
#define _PARSE_H_

#include <queue> 
#include <stack>
#include "tokens.h"
#include "composite.h"

class Parse{
  stack<TokenType> nesting_stack;
  queue<Token *> * token_queue;

public:  
  Parse(queue<Token *> *);
  Composite * parseProgram();

private:
  Composite * parseStatement();
  Composite * parseAssignment();
  Composite * parseExpression();
  Leaf      * parseConstant();
  Composite * parseBlock();
  Composite * parseLoopBlock();
  Composite * parseConditionalBlock();
};

#endif
