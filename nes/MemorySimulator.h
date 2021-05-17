#pragma once

#include <cstdint>
#include <Windows.h>
#include <memoryapi.h>
#include <sysinfoapi.h>

class MemorySimulator {

public:
	MemorySimulator();
	~MemorySimulator();
	uint8_t read(uint16_t);

private:
	uint8_t* mem;
	LPVOID lp;
	int numBytes = 100;

	uint8_t pcStartingAddr = 0x0000;
};