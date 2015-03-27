#include <vector>
#include <stack>
#include "tokens.h"
#include "composite.h"

Token * AbstractComposite::getData(){
	return data;
}

void AbstractComposite::setData(Token * _data){
	data = _data;
}

CompositeIterator::CompositeIterator(AbstractComposite * _root){
	root = _root;
	child_stack = new stack<AbstractComposite*>;
	index_stack = new stack<int>;

	child_stack->push(root);
	index_stack->push(0);
}

AbstractComposite * CompositeIterator::first(){
	return root;
}

AbstractComposite * CompositeIterator::next(){
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


CompositeIterator * AbstractComposite::iterator(){
  return new CompositeIterator(this);
}


Composite::Composite(Token * _data) : AbstractComposite(){
	data = _data;
}

AbstractComposite * Composite::getChild(unsigned int index){
	if(index < children.size()){
		return children[index];
	}else{
		return NULL;
	}
}

void Composite::addChild(AbstractComposite * child){
	children.push_back(child);
}

int Composite::size(){
	return children.size();
}



Leaf::Leaf(Token * _data) : AbstractComposite(){
	data = _data;
}

AbstractComposite * Leaf::getChild(unsigned int index){
	return NULL;
}

int Leaf::size(){
	return 0;
}


