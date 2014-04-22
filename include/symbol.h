#pragma once

#include <string>

using namespace std;

class Symbol
{
public:
	//TODO: add hash of attributes or something

	string id;
	string type;

	Symbol(string id, string type);

	string get_id();
	string get_type();


};

