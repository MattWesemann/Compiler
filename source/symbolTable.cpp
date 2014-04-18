#include "SymbolTable.h"
#include "Symbol.h"


using namespace std;

SymbolTable::SymbolTable() {
	
}

void SymbolTable::enterSymbol(string name, Type type) {
	/*
	Symbol symbol;
	symbol = Symbol(name, type);
	if (declaredLocally(symbol.getName())) {
		enterSymbol(symbol);
	}
	else {
		//throw an error for being declared twice
	}
	*/
}


/*
attributes SymbolTable::retrieveSymbol(string name) {
	return symbols[name]
}
*/


bool SymbolTable::declaredLocally(string name) {
	return false;	
}
