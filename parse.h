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
  AbstractComposite * parseStatement();
  Composite * parseAssignment();
  AbstractComposite * parseExpression();
  Composite * parseBlock();
  Composite * parseLoopBlock(Composite *);
  Composite * parseConditionalBlock(Composite * );
};

#endif
