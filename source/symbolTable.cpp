#include "SymbolTable.h"
#include "Symbol.h"
#include "namespace.h"
#include <exception>

using namespace std;

SymbolTable::SymbolTable() {
	
}

void SymbolTable::enterSymbol(string& name, Attributes& attributes) {
	if (!declaredLocally(name)) {
		symbols[name] = make_shared<Symbol>(Namespace::instance()->add(name), attributes);
	}
	else {
		throw exception(("Symbol already exists in table: " + name).c_str());
	}
}

shared_ptr<Symbol> SymbolTable::retrieveSymbol(string& name) {
	return symbols[name];
}

bool SymbolTable::declaredLocally(string& name) {
	return symbols.find(name) != symbols.end();	
}
