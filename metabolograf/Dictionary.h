#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <map>

#define _operator 1
#define _keyword 2
#define _variable 3
#define _function 4

class Dictionary
{
public:
	Dictionary()
	{
		operators["\""] = _keyword;
		operators["("] = _operator;
		operators[")"] = _operator;
		operators["["] = _operator;
		operators["]"] = _operator;
		operators["+"] = _operator;
		operators["-"] = _operator;
		operators["*"] = _operator;
		operators["/"] = _operator;
		operators["="] = _operator;
		operators[","] = _operator;
		operators[";"] = _operator;
		operators["&"] = _operator;

		functions["√–¿‘» "] = 1;
		functions["„‡ÙËÍ"] = 1;
		functions["plot"] = 1;
	};
	map<string, int> operators;
    map<string, int> functions;

};

#endif
