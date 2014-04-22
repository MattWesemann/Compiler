#pragma once
#include <string>

class Name {
public:
	Name(char* name, int len);
	std::string get();

private:
	char* name;
	int len;
};