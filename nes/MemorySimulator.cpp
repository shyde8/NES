#include "MemorySimulator.h"

MemorySimulator::MemorySimulator() {
	//mem = new uint8_t[numBytes];
	_SYSTEM_INFO s;
	GetSystemInfo(&s);
	lp = VirtualAlloc(s.lpMinimumApplicationAddress, 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	uint8_t* tmp = static_cast<uint8_t*>(lp);
	uint8_t* datatoset = tmp;
	
	*tmp = 0xA2;
	tmp++;
	*tmp = 0xFF;
	tmp++;
	*tmp = 0xB5;
	tmp++;
	*tmp = 0x80;
	tmp++;

	//*tmp = 0x4C;
	//tmp++;
	//*tmp = 0x02;
	//tmp++;
	//*tmp = 0xCD;

	datatoset += 0x007F;
	*datatoset = 0xCC;
	
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