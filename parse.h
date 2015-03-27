#ifndef _PARSE_H_
#define _PARSE_H_

#include <queue> 
#include <stack>
#include "tokens.h"
#include "parse.h"

void parseTokens(queue<Token *> *);
Composite * parseForLoop(queue<Token *> *);
Composite * parseConditional(queue<Token *> *);
Composite * parseBlock(queue<Token *> *);
Composite * parseStatement(queue<Token *> *);


#endif
