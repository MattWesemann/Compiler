#pragma once
#include "name.h"
#include <vector>
#include <string>

class Namespace {
public:
	Namespace();
	Name add(std::string name);

private:
	std::vector<char> bytes;
};