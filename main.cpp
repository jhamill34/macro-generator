#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include "tokens.h"
#include "composite.h"
#include "parse.h"

using namespace std;

int 
main(int argc, char * argv[])
{
  // Scan the file and create a lineup of Tokens
  queue<Token *> * token_queue = scanner(argv[1]);

  // Parse all the tokens and create an Abstract Syntax Tree (AST)
  Parse program_parser(token_queue);
  AbstractComposite * root = program_parser.parseProgram();
  
  // Create an iterator to loop through the AST 
  CompositeIterator * program_iterator = root->iterator();
  AbstractComposite * val = program_iterator->next(); 
  while(val != NULL){
    cout << val->getData()->value << endl;
    val = program_iterator->next();
  }

 return 0;
}

