#pragma once

#include <string>
#include "attributes.h"

class Symbol {
public:
	Symbol(std::string &name, Attributes& attributes);

	std::string getName();
	Attributes& getAttributes();

private:
	std::string name;
	Attributes attributes;
};

