#pragma once

#include <string>
#include "attributes.h"
#include "name.h"

class Symbol {
public:
	Symbol(Name &name, Attributes& attributes);

	std::string getName();
	Attributes& getAttributes();

private:
	Name name;
	Attributes attributes;
};

