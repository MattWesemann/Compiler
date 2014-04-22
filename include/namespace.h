#pragma once
#include "name.h"
#include <vector>
#include <string>

class Namespace {
public:
	static const int INIT_LEN = 128;

	Namespace();
	Name add(std::string name);

private:
	std::vector<char> bytes;
};