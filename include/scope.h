#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "symbolTable.h"

class SymbolTable;
class Symbol;

class Scope {
public:
	Scope();
	Scope(Scope* parent);

	std::shared_ptr<Symbol> getSymbol(std::string& name);
	void createSymbol(std::string& name, Attributes& attributes);

	std::shared_ptr<Scope> getScope(std::string& name);
	void createScope(std::string& name);

private:
	Scope* parent;
	SymbolTable table;
	std::unordered_map<std::string, std::shared_ptr<Scope>> names;
};