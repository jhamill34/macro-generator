#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include <vector>
#include <stack>
#include "tokens.h"

using namespace std;

// Forward declaration of CompositeIterator class
class CompositeIterator;

// Class to extend so we can create a composite design pattern
// TODO: make the composite pattern generic with templates
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

// Iterator class that we use to go through the tokens
// placed in the composite pattern so we can iterate through
// them in the correct order
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

  int getDepth();
};

// Has many AbstractComposite objects
class Composite : public AbstractComposite{
  // has many abstract composite objects
  vector<AbstractComposite *> children; // empty vector

public:
  Composite(Token * _data);

  AbstractComposite * getChild(unsigned int index);
  void addChild(AbstractComposite * child);
  void removeLastChild();
  void setChild(AbstractComposite *, unsigned int);
  int size();
};

// An end point to the composite pattern, holds primitive types (ie Number or String)
class Leaf : public AbstractComposite{

public:
  Leaf(Token * _data);
  AbstractComposite * getChild(unsigned int index);
  int size();
};

#endif
