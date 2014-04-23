#include "scope.h"

using namespace std;

Scope::Scope() : Scope(nullptr) {
}

Scope::Scope(Scope* parent){
	this->parent = parent;
}

shared_ptr<Symbol> Scope::getSymbol(string& name){
	auto ret = table.retrieveSymbol(name);

	// we found it
	if (ret.get() != nullptr)
		return ret;

	// we are global scope
	if (parent == nullptr)
		return shared_ptr<Symbol>();

	return parent->getSymbol(name);
}

void Scope::createSymbol(string& name, Attributes& attributes){
	table.enterSymbol(name, attributes);
}

shared_ptr<Scope> Scope::getScope(string& name){
	return names[name];
}

void Scope::createScope(string& name){
	names[name] = make_shared<Scope>(this);
}