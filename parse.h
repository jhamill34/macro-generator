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
  AbstractComposite * parseExpressionHelp(unsigned int);
  Composite * parseBlock();
  Composite * parseLoopBlock(Composite *);
  Composite * parseConditionalBlock(Composite * );
  Composite * parseInline();
  Composite * rotateLeft(Composite * tmp_head){
    AbstractComposite * tmp = tmp_head->getChild(1);
    tmp_head->removeLastChild();
    tmp_head->addChild(tmp->getChild(0));
    ((Composite *)tmp)->setChild(tmp_head, 0);
    tmp_head = (Composite *)tmp;
  
    return tmp_head;
  }
};

#endif
