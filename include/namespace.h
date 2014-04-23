#pragma once
#include "name.h"
#include <vector>
#include <string>

class Namespace {
public:
	Namespace();
	Name add(std::string name);
	std::string get(size_t offset, size_t len);

private:
	std::vector<char> bytes;
};