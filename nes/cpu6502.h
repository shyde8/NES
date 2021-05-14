#pragma once

#include <cstdint>

class cpu6502 {

public:

	// CONSTRUCTORS //
	cpu6502();


private:

	// REGISTERS //
	uint8_t a = 0x00;		// accumulator //
	uint8_t x = 0x00;		// x register //
	uint8_t y = 0x00;		// y register //
	uint8_t sp = 0x00;		// stack pointer //
	uint16_t pc = 0x0000;	// program counter //

	union p {				// status register //
		struct {
			unsigned int C : 1;		// carry //
			unsigned int Z : 1;		// zero //
			unsigned int I : 1;		// interrupt disable //
			unsigned int D : 1;		// decimal //
			unsigned int B : 2;		// B flag, unused //
			unsigned int V : 1;		// overflow //
			unsigned int N : 1;		// negative //
		};
		uint8_t pReg = 0x00;
	};
	p reg;
};
