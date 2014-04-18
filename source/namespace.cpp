#include "namespace.h"

using namespace std;

Namespace::Namespace(){

}

Symbol* Namespace::getSymbol(std::string name){
	return nullptr;
}

void Namespace::createSymbol(std::string name){

}

Namespace* Namespace::getNamespace(string name){
	return names[name];
}

void Namespace::createNamespace(string name){
	names[name] = new Namespace();
}