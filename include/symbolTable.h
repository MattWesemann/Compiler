#pragma once

#include <string>
#include <map>


using namespace std;

class SymbolTable
{
public:
	
	

	enum Type {
		Empty,
		Assignment,
		Block,
		Declaration,
		Declarations,
		Else,
		Expression,
		If,
		Literal,
		Operator,
		Program,
		Return,
		Symbol,
		While,
	};

	SymbolTable();


	// probably wont need these if we are doing one table per scope
	// create a new scope
	void openScope();
	//close the current scope
	void closeScope();


	// TODO : add attributes parameter to handle
	// values / modifiers ie. const static etc.
	// possibly make attributes a hash?
	// throw an error if symbol already in current scope
	void enterSymbol(string name, Type type);

	// is the symbol declared in the current scope.
	bool declaredLocally(string name);

	// returns attributes when declaredLocally is true
	// TODO : get attributes figured out
	// map retrieveSymbol(string name);


};
