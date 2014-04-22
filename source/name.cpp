#include "name.h"

using namespace std;

Name::Name(char* name, int len){
	this->name = name;
	this->len = len;
}

string Name::get(){
	return string(name, len);
}