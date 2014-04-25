#include "symbol.h"

using namespace std;

Symbol::Symbol(Name name, Attributes& attributes){
	this->name = name;
	this->attributes = attributes;
}

string Symbol::getName(){
	return name.get();
}

Attributes& Symbol::getAttributes(){
	return attributes;
}
