#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include <vector>
#include <stack>
#include "tokens.h"

using namespace std;

class CompositeIterator;

class AbstractComposite{
protected:
  Token * data;

public:
  friend class CompositeIterator;
  virtual AbstractComposite * getChild(unsigned int index) =0;  
  virtual int size() =0;
  virtual ~AbstractComposite(){};
  Token * getData();
  void setData(Token * _data);
  CompositeIterator * iterator();

};


class CompositeIterator{
  // Has-an abstract composite object to iterate
  AbstractComposite * root;
  // keep empty stacks to keep track of where 
  // we are in the search
  stack<AbstractComposite *> * child_stack;
  stack<int> * index_stack; 

public:
  CompositeIterator(AbstractComposite * _root);

  AbstractComposite * first();

  AbstractComposite * next();
};

class Composite : public AbstractComposite{
  // has many abstract composite objects
  vector<AbstractComposite *> children; // empty vector

public:
  Composite(Token * _data);

  AbstractComposite * getChild(unsigned int index);
  void addChild(AbstractComposite * child);
  int size();
};

class Leaf : public AbstractComposite{

public:
  Leaf(Token * _data);
  AbstractComposite * getChild(unsigned int index);
  int size();
};

#endif
