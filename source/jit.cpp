#include "jit.h"

size_t x86Jitter::getSize(){
	return size;
}

size_t x86Jitter::getDataOffset(){
	return dataOffset;
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

x86Jitter::x86Jitter(){
	buf = nullptr;
	size = 0;
	dataOffset = 0;

	SYSTEM_INFO info;
	GetSystemInfo(&info);
	pageSize = info.dwPageSize;
}

x86Jitter::~x86Jitter(){
	if (buf){
		VirtualFree(buf, 0, MEM_RELEASE);
		buf = nullptr;
	}
}

void* x86Jitter::allocateMemory(size_t codeSize, size_t dataSize){
	if (buf)
		return buf;

	dataOffset = pageSize;
	while (dataOffset < codeSize)
		dataOffset *= 2;

	size_t data = pageSize;
	while (data < dataSize)
		data *= 2;

	size = data + dataOffset;

	// initially only give read and write
	buf = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	return buf;
}

x86Jitter::JittedFunc x86Jitter::getFunction(){
	DWORD dummy;
	BOOL ret = VirtualProtect(buf, dataOffset, PAGE_EXECUTE_READ, &dummy);
	if (!ret)
		return nullptr;
	return (JittedFunc) buf;
}

#else
#include <unistd.h>
#include <sys/mman.h>

x86Jitter::x86Jitter(){
	buf = nullptr;
	size = 0;
	dataOffset = 0;
	pageSize = getpagesize();
}

x86Jitter::~x86Jitter(){
	if (buf){
		munmap(buf, size);
		buf = nullptr;
	}
}

void* x86Jitter::allocateMemory(size_t codeSize, size_t dataSize){
	if (buf)
		return buf;

	dataOffset = pageSize;
	while (dataOffset < codeSize)
		dataOffset *= 2;

	size_t data = pageSize;
	while (data < dataSize)
		data *= 2;

	size = data + dataOffset;

	mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

x86Jitter::JittedFunc x86Jitter::getFunction(){
	int ret = mprotect(buf, dataOffset, PROT_READ | PROT_EXEC);
	if (!ret)
		return nullptr;
	return (JittedFunc) buf;
}

#endif