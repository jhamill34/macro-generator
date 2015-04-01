#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include "tokens.h"
#include "composite.h"
#include "parse.h"
#include "execute.h"

using namespace std;

int 
main(int argc, char * argv[])
{  
  // Scan the file and create a lineup of Tokens
  queue<Token *> * token_queue = scanner(argv[1], NULL, "");
/*
  int i;
  for(i = 0; i < token_queue->size(); i++){
    Token * tmp = token_queue->front();
    cout << tmp->value << endl;
    token_queue->pop();
    token_queue->push(tmp);
  }
*/

  // Parse all the tokens and create an Abstract Syntax Tree (AST)

  Parse program_parser(token_queue);
  AbstractComposite * root = program_parser.parseProgram();
/*
  CompositeIterator * i = root->iterator();
  AbstractComposite * val = i->next();
  while(val != NULL){
    int j;
    for(j = 0; j < i->getDepth(); j++){
      cout << ">";
    }
    cout << val->getData()->value << endl;
    val = i->next();
  }
*/

  Execute program_execution(root);
  program_execution.executeProgram();

 
 return 0;
}

