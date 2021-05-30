#include "MemorySimulator.h"

MemorySimulator::MemorySimulator() {
	mem = new uint8_t[numBytes];
	start = mem;

	std::ifstream file;
	file.open("D:\\nestest.nes", std::ios::in | std::ios::binary);
	if (file) {

		char* buffer = new char[1];
		
		// skip first 10 bytes //
		uint16_t currOffset = 0x0000;
		while (currOffset < 0x0010) {
			file.read(buffer, 1);
			//std::cout << "read one byte!" << std::endl;
			currOffset++;
		}

		uint8_t* firstSet = start + 0x8000;
		uint8_t* secondSet = start + 0xC000;

		// load the next 0x4000 bytes into $8000-$BFFF and $C000-$FFFF //
		while (currOffset < 0x4010) {
			file.read(buffer, 1);

			*(firstSet) = buffer[0];
			*(secondSet) = buffer[0];

			firstSet++;
			secondSet++;

			currOffset++;
		}

		//std::cout << "firstSet: " << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << (int)(firstSet - start) << std::endl;
		//std::cout << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << (int)input << std::endl;

		file.close();
		delete[] buffer;
	}





	//_SYSTEM_INFO s;
	//GetSystemInfo(&s);
	//lp = VirtualAlloc(s.lpMinimumApplicationAddress, 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	//uint8_t* tmp = static_cast<uint8_t*>(lp);
	//uint8_t* datatoset = tmp;
	//uint8_t* orig = tmp;

	//*tmp = 0xA9;
	//tmp++;
	//*tmp = 0x6d;	
	//tmp++;
	//*tmp = 0x8D;
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
	delete[] mem;
	//_SYSTEM_INFO s;
	//GetSystemInfo(&s);
	//VirtualFree(s.lpMinimumApplicationAddress, NULL, MEM_RELEASE);
}

uint8_t MemorySimulator::read(uint16_t pc) {
	//int offset = pc - pcStartingAddr;
	//uint8_t* tmp = static_cast<uint8_t*>(lp);
	//return *(tmp + offset);
	//return *(mem + offset);

	return *(start + pc);
}

void MemorySimulator::write(uint8_t data, uint16_t addr) {
	*(start + addr) = data;
}