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
  Token * getData(){
    return data;
  }

  void setData(Token * _data){
    data = _data;
  }

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
  CompositeIterator(AbstractComposite * _root){
    root = _root;
    child_stack = new stack<AbstractComposite*>;
    index_stack = new stack<int>;

    child_stack->push(root);
    index_stack->push(0);
  }

  AbstractComposite * first(){
    return root;
  }

  AbstractComposite * next(){
    int cur_index = index_stack->top();
    AbstractComposite * current = child_stack->top();
    AbstractComposite * child = current->getChild(cur_index);
    while(child == NULL && child_stack->size() > 1){
      child_stack->pop();
      index_stack->pop();
      cur_index = index_stack->top() + 1;
      
      index_stack->pop();
      index_stack->push(cur_index);
      
      current = child_stack->top();
      child = current->getChild(cur_index);
    }
    if(child != NULL){ 
      child_stack->push(child);
      index_stack->push(0);
    }
    return child;
  }
};

CompositeIterator * AbstractComposite::iterator(){
  return new CompositeIterator(this);
}


class Composite : public AbstractComposite{
  // has many abstract composite objects
  vector<AbstractComposite *> children; // empty vector

public:
  Composite(Token * _data) : AbstractComposite(){
    data = _data;
  }

  AbstractComposite * getChild(unsigned int index){
    if(index < children.size()){
      return children[index];
    }else{
      return NULL;
    }
  }
  
  void addChild(AbstractComposite * child){
    children.push_back(child);
  }

  int size(){
    return children.size();
  }
};

class Leaf : public AbstractComposite{

public:
  Leaf(Token * _data) : AbstractComposite(){
    data = _data;
  }

  AbstractComposite * getChild(unsigned int index){
    return NULL;
  }

  int size(){
    return 0;
  }
};

#endif
