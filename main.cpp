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
  queue<Token *> * token_queue = scanner(argv[1]);
  //parseTokens(token_queue);


  Token * _t1 = new Token;
  _t1->value = "T1";
  _t1->type = SYMBOL;

  Token * _t2 = new Token;
  _t2->value = "T2";
  _t2->type = SYMBOL;
  
  Token * _t3 = new Token;
  _t3->value = "T3";
  _t3->type = SYMBOL;

  Token * _t4 = new Token;
  _t4->value = "T4";
  _t4->type = SYMBOL;

  Token * _t5 = new Token;
  _t5->value = "T5";
  _t5->type = SYMBOL;

  Composite * bob = new Composite(_t1);
  Composite * t = new Composite(_t2);
  Composite * t2 = new Composite(_t3);
  Composite * t5 = new Composite(_t5);

  Leaf * l = new Leaf(_t4);
  t2->addChild(t5);
  t2->addChild(l);
  
  bob->addChild(t);
  bob->addChild(t2);

  CompositeIterator * i = bob->iterator();
  AbstractComposite * val = i->next(); 
  while(val != NULL){
    cout << val->getData()->value << endl;
    val = i->next();
  }
  return 0;
}

