#ifndef _EXECUTE_H_
#define _EXECUTE_H_

#include <map>
#include <string>
#include "tokens.h"
#include "composite.h"

using namespace std;

class Execute{
	map<string, Token * > global_vars;
	AbstractComposite * root;
	//CompositeIterator * program_iterator;

public:
	Execute(AbstractComposite *);
	void executeProgram();

private:
	void execForLoop(AbstractComposite *, CompositeIterator*m);
	void execIfCondition(AbstractComposite *);
	void execBlock(AbstractComposite  *);
	void execPrint(Token * current);
	int toInteger(string value);
	int doMath(Token * t, CompositeIterator *);
	int execCondition(Token * current, CompositeIterator * iter);
};

#endif