#pragma once

class x86Jitter {
public:
	typedef void* (*JittedFunc)();

	x86Jitter();
	~x86Jitter();

	// returns buffer as a executable function
	JittedFunc getFunction();

	// returns a buffer to fill with code
	// code and data are both in this buffer with code being first.
	// the getFunction code will make the code part read and execute only
	void* allocateMemory(size_t codeSize, size_t dataSize);

	size_t getSize();
	size_t getDataOffset();

private:
	void* buf;
	size_t pageSize;
	size_t size;
	size_t dataOffset;
};