#include "scope.h"

using namespace std;

Scope::Scope(){

}

Symbol* Scope::getSymbol(std::string name){
	return nullptr;
}

void Scope::createSymbol(std::string name){

}

Scope* Scope::getScope(string name){
	return names[name];
}

void Scope::createScope(string name){
	names[name] = new Scope();
}