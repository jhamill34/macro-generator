#include <iostream>
#include "tokens.h"
#include "execute.h"

using namespace std;

Execute::Execute(AbstractComposite * _root){
	root = _root;
}

void Execute::executeProgram(){
	execBlock(root);
}

int Execute::doMath(Token * t, CompositeIterator * iter){
	if(t->type == IDENTIFIER){
		if(global_vars[t->value] != NULL){
			return toInteger(global_vars[t->value]->value);
		}else{
			cout << "undefind variable" << endl;
			exit(1);
		}
	}else if(t->type == NUMBER || t->type == STRING){
		return toInteger(t->value);
	}else{
		int val1 = doMath(iter->next()->getData(), iter);
		int val2 = doMath(iter->next()->getData(), iter);

		if(t->value[0] == ASTERISK){
			return val1 * val2;
		}else if(t->value[0] == FSLASH){
			return val1 / val2;
		}else if(t->value[0] == PLUS){
			return val1 + val2;
		}else if(t->value[0] == MINUS){
			return val1 - val2;
		}else if(t->value[0] == CARET){
			int i;
			int res = 1;
			for(i = 0; i < val2; i++){
				res *= val1;
			}
			return res;
		}
	}
	return 0;
}

int Execute::execCondition(Token * t, CompositeIterator * iter){
	//AbstractComposite * current = iter->next();
	if(t->type == IDENTIFIER){
		if(global_vars[t->value] != NULL){
			return toInteger(global_vars[t->value]->value);
		}else{
			cout << "undefind variable" << endl;
			exit(1);
		}
	}else if(t->type == NUMBER || t->type == STRING){
		return toInteger(t->value);
	}else{
		int val1 = execCondition(iter->next()->getData(), iter);
		int val2 = execCondition(iter->next()->getData(), iter);


		if(t->value.size() == 2 && t->value[0] == GT && t->value[1] == EQ){
			return val1 >= val2;
		}else if(t->value.size() == 2 && t->value[0] == LT && t->value[1] == EQ){
			return val1 <= val2;
		}else if(t->value.size() == 1 && t->value[0] == GT){
			return val1 > val2;
		}else if(t->value.size() == 1 && t->value[0] == LT){
			return val1 < val2;
		}else if(t->value.size() == 2 && t->value[0] == EQ && t->value[1] == EQ){
			return val1 == val2;
		}else if(t->value.size() == 1 && t->value[0] == BANG && t->value[1] == EQ){
			return val1 != val2;
		}else if(t->value.size() == 2 && t->value[0] == AND && t->value[1] == AND){
			return val1 && val2;
		}else if(t->value.size() == 2 && t->value[0] == PIPE && t->value[1] == PIPE){
			return val1 || val2;
		}else{
			return 0;
		}
	}
}

void Execute::execForLoop(AbstractComposite * forstart, CompositeIterator * iter){
	Token * current;
	AbstractComposite * condition_node = forstart->getChild(1);
	CompositeIterator * condition_iter = condition_node->iterator();

	AbstractComposite * repeat_node = forstart->getChild(2);
	CompositeIterator * repeat_iter = repeat_node->iterator();

	AbstractComposite * block_node = forstart->getChild(3);

	// Capture init
	// Set the value we are starting at from the first child
	current = iter->next()->getData();
	if(current->type == SYMBOL && current->value[0] == EQ && current->value.size() == 1){
		// TODO: implement scopes
		Token * key = iter->next()->getData();
		Token * val_t = iter->next()->getData();
		
		Token * result = new Token;
		result->type = val_t->type;
		result->col_num = -1;
		result->row_num = -1;
		result->priority = DEFAULT_PRIORTY;
		if(val_t->type == SYMBOL){
			result->value = to_string(doMath(val_t, iter));
		}else{
			result = val_t;
		}
		if(key->type == IDENTIFIER){
			global_vars[key->value] = result;
		} 
	}else{
		cout << "Runtime Error: Expected Assignment expression" << endl;
		exit(1);
	}

	while(execCondition(condition_node->getData(), condition_iter)){
		execBlock(block_node);
		condition_iter->reset();
		if(repeat_node->getData()->type == SYMBOL && repeat_node->getData()->value[0] == EQ && repeat_node->getData()->value.size() == 1){
			// TODO: implement scopes
			Token * key = repeat_iter->next()->getData();
			Token * val_t = repeat_iter->next()->getData();

			Token * result = new Token;
			result->type = val_t->type;
			result->col_num = -1;
			result->row_num = -1;
			result->priority = DEFAULT_PRIORTY;
			if(val_t->type == SYMBOL){
				result->value = to_string(doMath(val_t, repeat_iter));
			}else{
				result = val_t;
			}
			if(key->type == IDENTIFIER){
				global_vars[key->value] = result;
			} 
		}else{
			cout << "Runtime Error: Expected Assignment expression" << endl;
			exit(1);
		}	
		repeat_iter->reset();	
	}
}

void Execute::execIfCondition(CompositeIterator * iter){
	// TODO: Finish
}

void Execute::execElseCondition(CompositeIterator * iter){	
	// TODO: Finish
}


void Execute::execBlock(AbstractComposite * root){
	int i;
	for(i = 0; i < root->size(); i++){
		AbstractComposite * val = root->getChild(i);
		CompositeIterator * iter = val->iterator();
		Token * current = val->getData();

		if(current->type == FOR_LOOP_STATEMENT){
			execForLoop(val, iter);
		}else if(current->type == IF_CONDITION){
			execIfCondition(iter);
		}else if(current->type == SYMBOL && current->value[0] == EQ && current->value.size() == 1){
			// TODO: implement scopes
			Token * key = iter->next()->getData();
			Token * val_t = iter->next()->getData();
			
			Token * result = new Token;
			result->type = val_t->type;
			result->col_num = -1;
			result->row_num = -1;
			result->priority = DEFAULT_PRIORTY;
			if(val_t->type == SYMBOL){
				result->value = to_string(doMath(val_t, iter));
			}else{
				result = val_t;
			}
			if(key->type == IDENTIFIER){
				global_vars[key->value] = result;
			} 
		}else if(current->type == PRINT){
			execPrint(current);
		}else if(current->type == INLINE_START){
			Token * id = iter->next()->getData();
			if(global_vars[id->value] != NULL){
				cout << global_vars[id->value]->value;
			}else{
				cout << endl << "Runtime Error: trying to access undefined var" << endl;
				exit(1);
			}
		}
	}
}

void Execute::execPrint(Token * current){
	cout << current->value;
}

int Execute::toInteger(string value){
	int i, multiplier;
	int result = 0;
	multiplier = 1;
	for(i = (value.size() - 1); i >= 0; i--){
		result += (value[i] - '0') * multiplier;
		multiplier *= 10;
	}
	return result;
}