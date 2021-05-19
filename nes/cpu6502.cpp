#include "cpu6502.h"

// CONSTRUCTOR //
cpu6502::cpu6502() {
	
}

uint8_t cpu6502::readPC() {
	
	// FETCH //
	uint8_t inst = m->read(pc);
	std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)inst << std::endl;
	pc++;

	// DECODE //
	it = opcodeMatrix.find(inst);
	cycles = it->second.cycles;
	length = it->second.length;
	opcode = it->second.opcode;
	addrMode = it->second.addrMode;
	
	input = 0x0000;
	for (int i = 0; i < length - 1; i++) {
		input = input << 8;
		input |= m->read(pc);
		pc++;
	}
	std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)input << std::endl;

	// EXECUTE //
	exec_addr();
	exec_opcode();


	return 0;
}

void cpu6502::SetMemorySimulator(MemorySimulator* mem) {
	m = mem;
}

void cpu6502::display() {
	std::cout << "A=$" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)a;
	std::cout << " X=$" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)x;
	std::cout << " Y=$" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)y << std::endl;
	std::cout << "SP=$" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)sp;
	std::cout << " PC=$" << std::setfill('0') << std::setw(4) << std::right << std::hex << (int)pc << std::endl;
	std::cout << "NV-BDIZC" << std::endl;
	std::cout << reg.N << reg.V << "-" << reg.B << reg.D << reg.I << reg.Z << reg.C << std::endl;
}

void cpu6502::exec_addr() {
	return (this->*addrMode)();
}

void cpu6502::exec_opcode() {
	return (this->*opcode)();
}

// ADDRESSING MODES //
void cpu6502::ABS() {
	addr = input;
	data = m->read(addr);		// todo: different ranges must go to different devices //
}

void cpu6502::ABX() {
	addr = input + x;			// todo: oops cycle //
	data = m->read(addr);		// todo: different ranges must go to different devices //
}

void cpu6502::ABY() {
	addr = input + y;			// todo: oops cycle //
	data = m->read(addr);		// todo: different ranges must go to different devices //
}

void cpu6502::IDX() {
	uint8_t tmp = m->read(((input + x) % 256));
	addr = m->read(((input + x + 1) % 256));
	addr = addr << 8;
	addr |= tmp;
	data = m->read(addr);
}

void cpu6502::IDY() {
	uint8_t tmp = m->read(input);
	addr = m->read((input + 1) % 256);
	addr = addr << 8;
	addr |= tmp;
	addr = addr + y;
	data = m->read(addr);
}

void cpu6502::IMM() {
	data = input;
}

void cpu6502::IMP() {
	
}

void cpu6502::IND() {
	uint8_t tmp = m->read(input);
	addr = m->read(input + 1);
	addr = addr << 8;
	addr |= tmp;
}

void cpu6502::ZPG() {
	addr = input;
	data = m->read(addr);		// todo: different ranges must go to different devices //
}

void cpu6502::ZPX() {
	addr = ((input + x) % 256);
	data = m->read(addr);		// todo: different ranges must go to different devices //
}

void cpu6502::ZPY() {
	addr = ((input + y) % 256);
	data = m->read(addr);		// todo: different ranges must go to different devices //
}

// OPCODES //
void cpu6502::AND() {

	a = a & data;

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::CLC() {
	reg.C = 0;
}

void cpu6502::CLD() {
	reg.D = 0;
}

void cpu6502::CLI() {
	reg.I = 0;
}

void cpu6502::CLV() {
	reg.V = 0;
}

void cpu6502::CMP() {

	// C //
	if (a >= data)
		reg.C = 1;

	// Z //
	if (a == data)
		reg.Z = 1;

	// N //
	if ((a - data) & (1 << 7))
		reg.N = 1;
}

void cpu6502::CPX() {

	// C //
	if (x >= data)
		reg.C = 1;

	// Z //
	if (x == data)
		reg.Z = 1;

	// N //
	if ((x - data) & (1 << 7))
		reg.N = 1;
}

void cpu6502::CPY() {

	// C //
	if (y >= data)
		reg.C = 1;

	// Z //
	if (y == data)
		reg.Z = 1;

	// N //
	if ((y - data) & (1 << 7))
		reg.N = 1;
}

void cpu6502::DEX() {
	
	x--;

	// Z //
	if (x & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (x & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::DEY() {

	y--;

	// Z //
	if (y & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (y & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::EOR() {

	a = (a ^ data);

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;

}

void cpu6502::INX() {

	x++;

	// Z //
	if (x & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (x & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;

}

void cpu6502::INY() {

	y++;

	// Z //
	if (y & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (y & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;

}

void cpu6502::JMP() {

	pc = addr;
}

void cpu6502::LDA() {
	
	a = data;

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::LDX() {

	x = data;

	// Z //
	if (x & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (x & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::LDY() {

	y = data;

	// Z //
	if (y & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (y & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::NOP() {
	// no changes aside from incrementing PC by virtue of calling the instruction //
}

void cpu6502::ORA() {

	a = a | data;

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::PHA() {	
	*(reinterpret_cast<uint8_t*>(0x10100 | sp)) = a;		// todo: should be 0x0100 | sp
	sp--;
}

void cpu6502::PHP() {
	*(reinterpret_cast<uint8_t*>(0x10100 | sp)) = reg.pReg;		// todo: should be 0x0100 | sp
	sp--;
}

void cpu6502::PLA() {
	sp++;
	a = *(reinterpret_cast<uint8_t*>(0x10100 | sp));		// todo: should be 0x0100 | sp	

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::PLP() {
	sp++;
	reg.pReg = *(reinterpret_cast<uint8_t*>(0x10100 | sp));		// todo: should be 0x0100 | sp	
}

void cpu6502::SEC() {
	reg.C = 1;
}

void cpu6502::SED() {
	reg.D = 1;
}

void cpu6502::SEI() {
	reg.I = 1;
}

void cpu6502::TAX() {

	x = a;

	// Z //
	if (x & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (x & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::TAY() {

	y = a;

	// Z //
	if (y & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (y & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::TSX() {
	x = sp;
}

void cpu6502::TXA() {

	a = x;

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::TXS() {
	sp = x;
}

void cpu6502::TYA() {

	a = y;

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

