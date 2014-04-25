#include "name.h"
#include "namespace.h"

using namespace std;

Name::Name() : offset(0) , len(0){}

Name::Name(size_t offset, size_t len){
	this->offset = offset;
	this->len = len;
}

string Name::get(){
	return Namespace::instance()->get(offset, len);
}