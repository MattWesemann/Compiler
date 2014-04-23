#pragma once
#include <string>

class Namespace;

class Name {
public:
	Name();
	Name(size_t offset, size_t len, Namespace* space);
	std::string get();

private:
	size_t offset;
	size_t len;
	Namespace* space;
};