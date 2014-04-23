#include "name.h"
#include "namespace.h"

using namespace std;

Name::Name() : Name(0, 0, nullptr){}

Name::Name(size_t offset, size_t len, Namespace* space){
	this->offset = offset;
	this->len = len;
	this->space = space;
}

string Name::get(){
	return space->get(offset, len);
}