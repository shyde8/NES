#pragma once

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <map>
#include <functional>
#include "MemorySimulator.h"

class cpu6502 {

public:

	// CONSTRUCTORS //
	cpu6502();

	void display();

	uint8_t readPC();

	void SetMemorySimulator(MemorySimulator*);

private:

	typedef void(cpu6502::* opcodePtr)();
	typedef void(cpu6502::* addrModePtr)();

	struct Instruction {
		Instruction(int _length, int _cycles, opcodePtr _opcode, addrModePtr _addrMode) {
			length = _length;
			cycles = _cycles;
			opcode = _opcode;
			addrMode = _addrMode;
		}

		int length;					// length in bytes of the instruction, including the initial identifier byte //
		int cycles;					// number of cycles needed to execute instruction //
		opcodePtr opcode;			// function pointer for opcode //
		addrModePtr addrMode;		// function pointer for addressing mode //
	};

	MemorySimulator* m;

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

	// ADDRESSING MODES //
	void ABS();
	void IMM();
	void IMP();

	// OPCODES //
	void CLC();
	void CLD();
	void CLI();
	void CLV();
	void DEX();
	void DEY();
	void INX();
	void INY();
	void LDA();
	void LDX();
	void LDY();
	void SEC();
	void SED();
	void SEI();
	void TAX();
	void TAY();
	void TXA();
	void TYA();	

	// CURR INST STATE //
	int currCycles = 0;
	int cycles;					// total number of cycles needed to execute the fetched instruction //
	int length;					// length in bytes of the fetched instruction, including the initial identifier byte //
	opcodePtr opcode;			// function pointer for microcode of fetched instruction //
	addrModePtr addrMode;		// function pointer for addressing mode of fetched instruction //
	uint16_t input;				// remaining instruction input, can be either 0, 1, or 2 bytes long //
	uint16_t addr;				// if input represents a memory address, this variable will store the memory address //
	uint16_t data;				// if input represents data, this variable will store the data //

	void exec_addr();			// invokes the addressing mode function pointer //
	void exec_opcode();			// invokes the opcode function pointer //

	// OPCODE MAP //
	std::map<uint8_t, Instruction>::iterator it;
	std::map<uint8_t, Instruction> opcodeMatrix =
	{
		{0x18, Instruction(1, 2, &cpu6502::CLC, &cpu6502::IMP)},
		{0x38, Instruction(1, 2, &cpu6502::SEC, &cpu6502::IMP)},
		{0x58, Instruction(1, 2, &cpu6502::CLI, &cpu6502::IMP)},
		{0x78, Instruction(1, 2, &cpu6502::SEI, &cpu6502::IMP)},
		{0x88, Instruction(1, 2, &cpu6502::DEY, &cpu6502::IMP)},
		{0x8A, Instruction(1, 2, &cpu6502::TXA, &cpu6502::IMP)},
		{0x98, Instruction(1, 2, &cpu6502::TYA, &cpu6502::IMP)},
		{0xA0, Instruction(2, 2, &cpu6502::LDY, &cpu6502::IMM)},
		{0xA2, Instruction(2, 2, &cpu6502::LDX, &cpu6502::IMM)},
		{0xA8, Instruction(1, 2, &cpu6502::TAY, &cpu6502::IMP)},
		{0xA9, Instruction(2, 2, &cpu6502::LDA, &cpu6502::IMM)},
		{0xAA, Instruction(1, 2, &cpu6502::TAX, &cpu6502::IMP)},
		{0xAD, Instruction(3, 4, &cpu6502::LDA, &cpu6502::ABS)},
		{0xB8, Instruction(1, 2, &cpu6502::CLV, &cpu6502::IMP)},
		{0xC8, Instruction(1, 2, &cpu6502::INY, &cpu6502::IMP)},
		{0xCA, Instruction(1, 2, &cpu6502::DEX, &cpu6502::IMP)},
		{0xD8, Instruction(1, 2, &cpu6502::CLD, &cpu6502::IMP)},
		{0xE8, Instruction(1, 2, &cpu6502::INX, &cpu6502::IMP)},
		{0xF8, Instruction(1, 2, &cpu6502::SED, &cpu6502::IMP)},
	};
};
