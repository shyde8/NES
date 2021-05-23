#include "MemorySimulator.h"

MemorySimulator::MemorySimulator() {
	//mem = new uint8_t[numBytes];
	_SYSTEM_INFO s;
	GetSystemInfo(&s);
	lp = VirtualAlloc(s.lpMinimumApplicationAddress, 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	uint8_t* tmp = static_cast<uint8_t*>(lp);
	uint8_t* datatoset = tmp;
	uint8_t* orig = tmp;

	*tmp = 0xA9;
	tmp++;
	*tmp = 0x6D;	
	tmp++;
	*tmp = 0x4A;
	tmp++;

	//*tmp = 0x2C;
	//tmp++;
	//*tmp = 0xFF;
	//tmp++;
	//*tmp = 0x02;
	//tmp++;

	
	//datatoset += 0x02ff;
	//*datatoset = 0x6D;
	//datatoset++;
	//*datatoset = 0xDD;
	//datatoset++;
	//*datatoset = 0x60;
	//datatoset = orig;
	//datatoset += 0x0403;
	//*datatoset = 0x0e;	
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