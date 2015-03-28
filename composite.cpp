#include <vector>
#include <stack>
#include "tokens.h"
#include "composite.h"

/**
* function to return the data currently in the current node
* @return Token *
*/
Token * AbstractComposite::getData(){
	return data;
}

/** Function to set the data currently in the composite node
* @param Token * _data
*/
void AbstractComposite::setData(Token * _data){
	data = _data;
}

/** Constructor for the iterator object
* @param AbstractComposite * _root 
*/
CompositeIterator::CompositeIterator(AbstractComposite * _root){
	root = _root;
	child_stack = new stack<AbstractComposite*>;
	index_stack = new stack<int>;

	child_stack->push(root);
	index_stack->push(0);
}

/** Function to return the "first" node in the composite structure
* @return AbstractComposite * root
*/
AbstractComposite * CompositeIterator::first(){
	return root;
}

/** Function to return the next node in the list
* @return AbstractComposite * next
*/
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

/** Function to return an iterator object based off of the current instance
* @returns CompositeIterator * 
*/
CompositeIterator * AbstractComposite::iterator(){
  return new CompositeIterator(this);
}

/** Composite Constructor 
* @parent AbstractComposite
* @param Token * _data
*/
Composite::Composite(Token * _data) : AbstractComposite(){
	data = _data;
}

/** Function to get the nth index of the children that are stored internally
* the order is based off the order in which they were added
* @param int index
* @returns AbstractComposite * 
*/
AbstractComposite * Composite::getChild(unsigned int index){
	if(index < children.size()){
		return children[index];
	}else{
		return NULL;
	}
}

/** Function to insert a child to this Object
* @param AbstractComposite * child
*/
void Composite::addChild(AbstractComposite * child){
	children.push_back(child);
}

/** Function that returns the current number of children
* @returns int size
*/
int Composite::size(){
	return children.size();
}

/** Leaf constructor 
* @parent AbstractComposite 
* @param Token * _data 
*/
Leaf::Leaf(Token * _data) : AbstractComposite(){
	data = _data;
}

/** Function implemented only to adhere to the Base Class 
* @param int index
* @returns NULL >>always<<
*/
AbstractComposite * Leaf::getChild(unsigned int index){
	return NULL;
}

/** Object can't hold any children so always returns 0
* @returns 0
*/
int Leaf::size(){
	return 0;
}


