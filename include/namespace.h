#pragma once

#include <unordered_map>
#include <string>

class SymbolTable;
class Symbol;

class Namespace {
public:
	Namespace();

	Symbol* getSymbol(std::string name);
	void createSymbol(std::string name);

	Namespace* getNamespace(std::string name);
	void createNamespace(std::string name);

private:
	SymbolTable* table;
	std::unordered_map<std::string, Namespace*> names;
};