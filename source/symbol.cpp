#include "Symbol.h"

using namespace std;

Symbol::Symbol(string& name, Attributes& attributes){
	this->name = name;
	this->attributes = attributes;
}

string Symbol::getName(){
	return name;
}

Attributes& Symbol::getAttributes(){
	return attributes;
}
