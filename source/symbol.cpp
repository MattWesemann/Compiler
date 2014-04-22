#include "Symbol.h"


Symbol::Symbol(string _id, string _type)
{
	id = _id;
	type = _type;
}


string Symbol::get_id() 
{
	return id;
}

string Symbol::get_type()
{
	return type;
}
