#pragma once

#include <string>

class Attributes {
public:
	Attributes();
	std::string type;
	bool isConst;
	int memLoc;
};