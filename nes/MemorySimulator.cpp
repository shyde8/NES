#include "MemorySimulator.h"

MemorySimulator::MemorySimulator() {
	//mem = new uint8_t[numBytes];
	_SYSTEM_INFO s;
	GetSystemInfo(&s);
	lp = VirtualAlloc(s.lpMinimumApplicationAddress, 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	uint8_t* tmp = static_cast<uint8_t*>(lp);
	uint8_t* datatoset = tmp;
	
	*tmp = 0xAD;
	tmp++;
	*tmp = 0x00;
	tmp++;
	*tmp = 0x80;

	datatoset += 0x0080;
	*datatoset = 0xFE;
	
	//uint8_t* tmp = mem;	
	
	//*tmp = 0x38;		//c
	//tmp++;
	
}

MemorySimulator::~MemorySimulator() {
	//delete[] mem;
	_SYSTEM_INFO s;
	GetSystemInfo(&s);
	VirtualFree(s.lpMinimumApplicationAddress, NULL, MEM_RELEASE);
}

uint8_t MemorySimulator::read(uint16_t pc) {
	int offset = pc - pcStartingAddr;
	uint8_t* tmp = static_cast<uint8_t*>(lp);
	return *(tmp + offset);
	//return *(mem + offset);
}