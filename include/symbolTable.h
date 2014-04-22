#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "attributes.h"
#include "symbol.h"

class SymbolTable {
public:
	SymbolTable();

	// probably wont need these if we are doing one table per scope
	// create a new scope
	void openScope();
	//close the current scope
	void closeScope();

	// possibly make attributes a hash?
	// throw an error if symbol already in current scope
	void enterSymbol(std::string& name, Attributes& attributes);

	// is the symbol declared in the current scope.
	bool declaredLocally(std::string& name);

	std::shared_ptr<Symbol> retrieveSymbol(std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;
};
