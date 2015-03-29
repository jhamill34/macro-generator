#include <iostream>
#include "tokens.h"
#include "execute.h"

using namespace std;

Execute::Execute(AbstractComposite * _root){
	root = _root;
}

void Execute::executeProgram(){
	program_iterator = root->iterator();
	AbstractComposite * val = program_iterator->next();
	while(val != NULL){
		Token * current = val->getData();

		if(current->type == FOR_LOOP_STATEMENT){
			execForLoop();
		}else if(current->type == IF_CONDITION){
			execIfCondition();
		}else if(current->type == SYMBOL && current->value[0] == EQ && current->value.size() == 1){
			// TODO: implement scopes
			Token * key = program_iterator->next()->getData();
			Token * val_t = program_iterator->next()->getData();
			
			Token * result = new Token;
			result->type = val_t->type;
			result->col_num = -1;
			result->row_num = -1;
			result->priority = DEFAULT_PRIORTY;
			if(val_t->type == SYMBOL){
				result->value = to_string(doMath(val_t));
			}else{
				result = val_t;
			}
			if(key->type == IDENTIFIER){
				global_vars[key->value] = result;
			} 
		}else if(current->type == PRINT){
			execPrint(current);
		}else if(current->type == INLINE_START){
			Token * id = program_iterator->next()->getData();
			if(global_vars[id->value] != NULL){
				cout << global_vars[id->value]->value;
			}else{
				cout << endl << "Runtime Error: trying to access undefined var" << endl;
				exit(1);
			}
		}

		val = program_iterator->next();
	}
}

int Execute::doMath(Token * t){
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
		int val1 = doMath(program_iterator->next()->getData());
		int val2 = doMath(program_iterator->next()->getData());

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

void Execute::execForLoop(){
	cout << "EXECUTE FOR LOOP" << endl;
}

void Execute::execIfCondition(){
	cout << "EXECT IF CONDITION" << endl;
}

void Execute::execElseCondition(){	
	cout << "EXECUTE ELSE" << endl;
}

void Execute::execAssignment(){
	cout << "ASSIGNING VARIABLE" << endl;
}

void Execute::execBlock(){
	cout << "EXECUTING BLOCK CLODE" << endl;
}

void Execute::execExpression(){
	cout << "EXECUTE EXPRESSION" << endl;
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
