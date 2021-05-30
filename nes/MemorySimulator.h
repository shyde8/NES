#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <memoryapi.h>
#include <sysinfoapi.h>

class MemorySimulator {

public:
	MemorySimulator();
	~MemorySimulator();
	uint8_t read(uint16_t);
	void write(uint8_t data, uint16_t addr);

private:
	uint8_t* mem;
	uint8_t* start;
	LPVOID lp;
	int numBytes = 0xFFFF;

	uint8_t pcStartingAddr = 0x0000;
};