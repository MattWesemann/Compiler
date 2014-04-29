#pragma once
#include "name.h"
#include <vector>
#include <string>

class Namespace {
public:
	Name add(std::string name);
	std::string get(size_t offset, size_t len);

	static Namespace* instance();
	void clear();

private:
	Namespace();
	static Namespace* singleton;
	std::vector<char> bytes;
};