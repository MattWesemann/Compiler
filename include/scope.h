#pragma once

#include <unordered_map>
#include <string>

class SymbolTable;
class Symbol;

class Scope {
public:
	Scope();

	Symbol* getSymbol(std::string name);
	void createSymbol(std::string name);

	Scope* getScope(std::string name);
	void createScope(std::string name);

private:
	SymbolTable* table;
	std::unordered_map<std::string, Scope*> names;
};