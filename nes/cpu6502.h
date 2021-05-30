#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
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

	std::ofstream log;

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
	uint8_t sp = 0xFF;		// stack pointer //
	uint16_t pc = 0xC000;	// program counter //

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
	void ABX();
	void ABY();
	void IDX();
	void IDY();
	void IMM();
	void IMP();
	void IND();
	void REL();
	void ZPG();
	void ZPX();
	void ZPY();

	// OPCODES //
	void ADC();
	void AND();
	void ASL();
	void BCC();
	void BCS();
	void BEQ();
	void BIT();
	void BMI();
	void BNE();
	void BPL();
	void BRK();
	void BVC();
	void BVS();
	void CLC();
	void CLD();
	void CLI();
	void CLV();
	void CMP();
	void CPX();
	void CPY();
	void DEC();
	void DEX();
	void DEY();
	void EOR();
	void INC();
	void INX();
	void INY();
	void JMP();
	void JSR();
	void LDA();
	void LDX();
	void LDY();
	void LSR();
	void NOP();
	void ORA();
	void PHA();
	void PHP();
	void PLA();
	void PLP();
	void ROL();
	void ROR();
	void RTI();
	void RTS();
	void SBC();
	void SEC();
	void SED();
	void SEI();
	void STA();
	void STX();
	void STY();
	void TAX();
	void TAY();
	void TSX();
	void TXA();
	void TXS();
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

	uint8_t read(uint16_t addr);
	void write(uint8_t data, uint16_t addr);

	// OPCODE MAP //
	std::map<uint8_t, Instruction>::iterator it;
	std::map<uint8_t, Instruction> opcodeMatrix =
	{
		{0x00, Instruction(1, 7, &cpu6502::BRK, &cpu6502::IMP)},
		{0x01, Instruction(2, 6, &cpu6502::ORA, &cpu6502::IDX)},
		{0x05, Instruction(2, 3, &cpu6502::ORA, &cpu6502::ZPG)},
		{0x06, Instruction(2, 5, &cpu6502::ASL, &cpu6502::ZPG)},
		{0x08, Instruction(1, 3, &cpu6502::PHP, &cpu6502::IMP)},
		{0x09, Instruction(2, 2, &cpu6502::ORA, &cpu6502::IMM)},
		{0x0A, Instruction(1, 2, &cpu6502::ASL, &cpu6502::IMP)},
		{0x0D, Instruction(3, 4, &cpu6502::ORA, &cpu6502::ABS)},
		{0x0E, Instruction(3, 6, &cpu6502::ASL, &cpu6502::ABS)},
		{0x10, Instruction(2, 2, &cpu6502::BPL, &cpu6502::REL)},
		{0x11, Instruction(2, 5, &cpu6502::ORA, &cpu6502::IDY)},
		{0x15, Instruction(2, 4, &cpu6502::ORA, &cpu6502::ZPX)},
		{0x16, Instruction(2, 6, &cpu6502::ASL, &cpu6502::ZPX)},
		{0x18, Instruction(1, 2, &cpu6502::CLC, &cpu6502::IMP)},
		{0x19, Instruction(3, 4, &cpu6502::ORA, &cpu6502::ABY)},
		{0x1E, Instruction(3, 7, &cpu6502::ASL, &cpu6502::ABX)},
		{0x1D, Instruction(3, 4, &cpu6502::ORA, &cpu6502::ABX)},
		{0x20, Instruction(3, 6, &cpu6502::JSR, &cpu6502::ABS)},
		{0x21, Instruction(2, 6, &cpu6502::AND, &cpu6502::IDX)},
		{0x24, Instruction(2, 3, &cpu6502::BIT, &cpu6502::ZPG)},
		{0x25, Instruction(2, 3, &cpu6502::AND, &cpu6502::ZPG)},
		{0x26, Instruction(2, 5, &cpu6502::ROL, &cpu6502::ZPG)},
		{0x28, Instruction(1, 4, &cpu6502::PLP, &cpu6502::IMP)},
		{0x29, Instruction(2, 2, &cpu6502::AND, &cpu6502::IMM)},
		{0x2A, Instruction(1, 2, &cpu6502::ROL, &cpu6502::IMP)},
		{0x2C, Instruction(3, 4, &cpu6502::BIT, &cpu6502::ABS)},
		{0x2D, Instruction(3, 4, &cpu6502::AND, &cpu6502::ABS)},
		{0x2E, Instruction(3, 6, &cpu6502::ROL, &cpu6502::ABS)},
		{0x30, Instruction(2, 2, &cpu6502::BMI, &cpu6502::REL)},
		{0x31, Instruction(2, 5, &cpu6502::AND, &cpu6502::IDY)},
		{0x35, Instruction(2, 4, &cpu6502::AND, &cpu6502::ZPX)},
		{0x36, Instruction(2, 6, &cpu6502::ROL, &cpu6502::ZPX)},
		{0x38, Instruction(1, 2, &cpu6502::SEC, &cpu6502::IMP)},
		{0x39, Instruction(3, 4, &cpu6502::AND, &cpu6502::ABY)},
		{0x3D, Instruction(3, 4, &cpu6502::AND, &cpu6502::ABX)},
		{0x3E, Instruction(3, 7, &cpu6502::ROL, &cpu6502::ABX)},
		{0x40, Instruction(1, 6, &cpu6502::RTI, &cpu6502::IMP)},
		{0x41, Instruction(2, 6, &cpu6502::EOR, &cpu6502::IDX)},
		{0x45, Instruction(2, 3, &cpu6502::EOR, &cpu6502::ZPG)},
		{0x46, Instruction(2, 5, &cpu6502::LSR, &cpu6502::ZPG)},
		{0x48, Instruction(1, 3, &cpu6502::PHA, &cpu6502::IMP)},
		{0x49, Instruction(2, 2, &cpu6502::EOR, &cpu6502::IMM)},
		{0x4A, Instruction(1, 2, &cpu6502::LSR, &cpu6502::IMP)},
		{0x4C, Instruction(3, 3, &cpu6502::JMP, &cpu6502::ABS)},
		{0x4D, Instruction(3, 4, &cpu6502::EOR, &cpu6502::ABS)},
		{0x4E, Instruction(3, 6, &cpu6502::LSR, &cpu6502::ABS)},
		{0x50, Instruction(2, 2, &cpu6502::BVC, &cpu6502::REL)},
		{0x51, Instruction(2, 5, &cpu6502::EOR, &cpu6502::IDY)},
		{0x55, Instruction(2, 4, &cpu6502::EOR, &cpu6502::ZPX)},
		{0x56, Instruction(2, 6, &cpu6502::LSR, &cpu6502::ZPX)},
		{0x58, Instruction(1, 2, &cpu6502::CLI, &cpu6502::IMP)},
		{0x59, Instruction(3, 4, &cpu6502::EOR, &cpu6502::ABY)},
		{0x5D, Instruction(3, 4, &cpu6502::EOR, &cpu6502::ABX)},
		{0x5E, Instruction(3, 7, &cpu6502::LSR, &cpu6502::ABX)},
		{0x60, Instruction(1, 6, &cpu6502::RTS, &cpu6502::IMP)},
		{0x61, Instruction(2, 6, &cpu6502::ADC, &cpu6502::IDX)},
		{0x65, Instruction(2, 3, &cpu6502::ADC, &cpu6502::ZPG)},
		{0x66, Instruction(2, 5, &cpu6502::ROR, &cpu6502::ZPG)},
		{0x68, Instruction(1, 4, &cpu6502::PLA, &cpu6502::IMP)},
		{0x69, Instruction(2, 2, &cpu6502::ADC, &cpu6502::IMM)},
		{0x6A, Instruction(1, 2, &cpu6502::ROR, &cpu6502::IMP)},
		{0x6C, Instruction(3, 5, &cpu6502::JMP, &cpu6502::IND)},
		{0x6D, Instruction(3, 4, &cpu6502::ADC, &cpu6502::ABS)},
		{0x6E, Instruction(3, 6, &cpu6502::ROR, &cpu6502::ABS)},
		{0x70, Instruction(2, 2, &cpu6502::BVS, &cpu6502::REL)},
		{0x71, Instruction(2, 5, &cpu6502::ADC, &cpu6502::IDY)},
		{0x75, Instruction(2, 4, &cpu6502::ADC, &cpu6502::ZPX)},
		{0x76, Instruction(2, 6, &cpu6502::ROR, &cpu6502::ZPX)},
		{0x78, Instruction(1, 2, &cpu6502::SEI, &cpu6502::IMP)},
		{0x79, Instruction(3, 4, &cpu6502::ADC, &cpu6502::ABY)},
		{0x7D, Instruction(3, 4, &cpu6502::ABX, &cpu6502::ABX)},
		{0x7E, Instruction(3, 7, &cpu6502::ROR, &cpu6502::ABX)},
		{0x81, Instruction(2, 6, &cpu6502::STA, &cpu6502::IDX)},
		{0x84, Instruction(2, 3, &cpu6502::STY, &cpu6502::ZPG)},
		{0x85, Instruction(2, 3, &cpu6502::STA, &cpu6502::ZPG)},
		{0x86, Instruction(2, 3, &cpu6502::STX, &cpu6502::ZPG)},
		{0x88, Instruction(1, 2, &cpu6502::DEY, &cpu6502::IMP)},
		{0x8A, Instruction(1, 2, &cpu6502::TXA, &cpu6502::IMP)},
		{0x8C, Instruction(3, 4, &cpu6502::STY, &cpu6502::ABS)},
		{0x8D, Instruction(3, 4, &cpu6502::STA, &cpu6502::ABS)},
		{0x8E, Instruction(3, 4, &cpu6502::STX, &cpu6502::ABS)},
		{0x90, Instruction(2, 2, &cpu6502::BCC, &cpu6502::REL)},
		{0x91, Instruction(2, 6, &cpu6502::STA, &cpu6502::IDY)},
		{0x94, Instruction(2, 4, &cpu6502::STY, &cpu6502::ZPX)},
		{0x95, Instruction(2, 4, &cpu6502::STA, &cpu6502::ZPX)},
		{0x96, Instruction(2, 4, &cpu6502::STX, &cpu6502::ZPY)},
		{0x98, Instruction(1, 2, &cpu6502::TYA, &cpu6502::IMP)},
		{0x99, Instruction(3, 5, &cpu6502::STA, &cpu6502::ABY)},
		{0x9A, Instruction(1, 2, &cpu6502::TXS, &cpu6502::IMP)},
		{0x9D, Instruction(3, 5, &cpu6502::STA, &cpu6502::ABX)},
		{0xA0, Instruction(2, 2, &cpu6502::LDY, &cpu6502::IMM)},
		{0xA1, Instruction(2, 6, &cpu6502::LDA, &cpu6502::IDX)},
		{0xA2, Instruction(2, 2, &cpu6502::LDX, &cpu6502::IMM)},
		{0xA4, Instruction(2, 3, &cpu6502::LDY, &cpu6502::ZPG)},
		{0xA5, Instruction(2, 3, &cpu6502::LDA, &cpu6502::ZPG)},
		{0xA6, Instruction(2, 3, &cpu6502::LDX, &cpu6502::ZPG)},
		{0xA8, Instruction(1, 2, &cpu6502::TAY, &cpu6502::IMP)},
		{0xA9, Instruction(2, 2, &cpu6502::LDA, &cpu6502::IMM)},
		{0xAA, Instruction(1, 2, &cpu6502::TAX, &cpu6502::IMP)},
		{0xAC, Instruction(3, 4, &cpu6502::LDY, &cpu6502::ABS)},
		{0xAD, Instruction(3, 4, &cpu6502::LDA, &cpu6502::ABS)},
		{0xAE, Instruction(3, 4, &cpu6502::LDX, &cpu6502::ABS)},
		{0xB0, Instruction(2, 2, &cpu6502::BCS, &cpu6502::REL)},
		{0xB1, Instruction(2, 5, &cpu6502::LDA, &cpu6502::IDY)},
		{0xB4, Instruction(2, 4, &cpu6502::LDY, &cpu6502::ZPX)},
		{0xB5, Instruction(2, 4, &cpu6502::LDA, &cpu6502::ZPX)},
		{0xB6, Instruction(2, 4, &cpu6502::LDX, &cpu6502::ZPY)},
		{0xB8, Instruction(1, 2, &cpu6502::CLV, &cpu6502::IMP)},
		{0xB9, Instruction(3, 4, &cpu6502::LDA, &cpu6502::ABY)},
		{0xBA, Instruction(1, 2, &cpu6502::TSX, &cpu6502::IMP)},
		{0xBC, Instruction(3, 4, &cpu6502::LDY, &cpu6502::ABX)},
		{0xBD, Instruction(3, 4, &cpu6502::LDA, &cpu6502::ABX)},
		{0xBE, Instruction(3, 4, &cpu6502::LDX, &cpu6502::ABY)},
		{0xC0, Instruction(2, 2, &cpu6502::CPY, &cpu6502::IMM)},
		{0xC1, Instruction(2, 6, &cpu6502::CMP, &cpu6502::IDX)},
		{0xC4, Instruction(2, 3, &cpu6502::CPY, &cpu6502::ZPG)},
		{0xC5, Instruction(2, 3, &cpu6502::CMP, &cpu6502::ZPG)},
		{0xC6, Instruction(2, 5, &cpu6502::DEC, &cpu6502::ZPG)},
		{0xC8, Instruction(1, 2, &cpu6502::INY, &cpu6502::IMP)},
		{0xC9, Instruction(2, 2, &cpu6502::CMP, &cpu6502::IMM)},
		{0xCA, Instruction(1, 2, &cpu6502::DEX, &cpu6502::IMP)},
		{0xCC, Instruction(3, 4, &cpu6502::CPY, &cpu6502::ABS)},
		{0xCD, Instruction(3, 4, &cpu6502::CMP, &cpu6502::ABS)},
		{0xCE, Instruction(3, 6, &cpu6502::DEC, &cpu6502::ABS)},
		{0xD0, Instruction(2, 2, &cpu6502::BNE, &cpu6502::REL)},
		{0xD1, Instruction(2, 5, &cpu6502::CMP, &cpu6502::IDY)},
		{0xD5, Instruction(2, 4, &cpu6502::CMP, &cpu6502::ZPX)},
		{0xD6, Instruction(2, 6, &cpu6502::DEC, &cpu6502::ZPX)},
		{0xD8, Instruction(1, 2, &cpu6502::CLD, &cpu6502::IMP)},
		{0xD9, Instruction(3, 4, &cpu6502::CMP, &cpu6502::ABY)},
		{0xDD, Instruction(3, 4, &cpu6502::CMP, &cpu6502::ABX)},
		{0xDE, Instruction(3, 7, &cpu6502::DEC, &cpu6502::ABX)},
		{0xE0, Instruction(2, 2, &cpu6502::CPX, &cpu6502::IMM)},
		{0xE1, Instruction(2, 6, &cpu6502::SBC, &cpu6502::IDX)},
		{0xE4, Instruction(2, 3, &cpu6502::CPX, &cpu6502::ZPG)},
		{0xE5, Instruction(2, 3, &cpu6502::SBC, &cpu6502::ZPG)},
		{0xE6, Instruction(2, 5, &cpu6502::INC, &cpu6502::ZPG)},
		{0xE8, Instruction(1, 2, &cpu6502::INX, &cpu6502::IMP)},
		{0xE9, Instruction(2, 2, &cpu6502::SBC, &cpu6502::IMM)},
		{0xEA, Instruction(1, 2, &cpu6502::NOP, &cpu6502::IMP)},
		{0xEC, Instruction(3, 4, &cpu6502::CPX, &cpu6502::ABS)},
		{0xED, Instruction(3, 4, &cpu6502::SBC, &cpu6502::ABS)},
		{0xEE, Instruction(3, 6, &cpu6502::INC, &cpu6502::ABS)},
		{0xF0, Instruction(2, 2, &cpu6502::BEQ, &cpu6502::REL)},
		{0xF1, Instruction(2, 5, &cpu6502::SBC, &cpu6502::IDY)},
		{0xF5, Instruction(2, 4, &cpu6502::SBC, &cpu6502::ZPX)},
		{0xF6, Instruction(2, 6, &cpu6502::INC, &cpu6502::ZPX)},
		{0xF8, Instruction(1, 2, &cpu6502::SED, &cpu6502::IMP)},
		{0xF9, Instruction(3, 4, &cpu6502::SBC, &cpu6502::ABY)},
		{0xFD, Instruction(3, 4, &cpu6502::SBC, &cpu6502::ABX)},
		{0xFE, Instruction(3, 7, &cpu6502::INC, &cpu6502::ABX)},
	};
};
