#include "cpu6502.h"

// CONSTRUCTOR //
cpu6502::cpu6502() {
	this->log.open("D:\\log.txt", std::ios::out);
	this->reg.pReg = 0x24;
}

uint8_t cpu6502::readPC() {
	
	std::cout << std::setfill('0') << std::setw(4) << std::right << std::hex << (int)pc << std::endl;
	this->log << std::setfill('0') << std::setw(4) << std::right << std::hex << (int)pc;
	this->log << "                                            " << "A:" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)a << " X:" << (int)x << " Y:" << (int)y << " P:" << (int)reg.pReg << std::endl;

	// FETCH //
	uint8_t inst = m->read(pc);
	//std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)inst << std::endl;
	pc++;

	// DECODE //
	it = opcodeMatrix.find(inst);

	if (it == opcodeMatrix.end())
		return 0;

	cycles = it->second.cycles;
	length = it->second.length;
	opcode = it->second.opcode;
	addrMode = it->second.addrMode;
	
	input = 0x0000;
	for (int i = 0; i < length - 1; i++) {
		
		// todo: make this not disgusting //

		if (i == 0) {
			input |= m->read(pc);
		}
		else {
			input |= (m->read(pc) << 8);
		}

		pc++;
	}
	//std::cout << "0x" << std::setfill('0') << std::setw(4) << std::right << std::hex << (int)input << std::endl;

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

uint8_t cpu6502::read(uint16_t addr) {

	return 0;
}

void cpu6502::write(uint8_t data, uint16_t addr) {
	m->write(data, addr);
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

void cpu6502::REL() {
	uint8_t tmp = input;
	int8_t offset = (tmp ^ 0x80) - 0x80;
	//std::cout << "offset="  << static_cast<int16_t>(offset);
	//addr = (pc - 1) + offset;
	addr = pc + offset;
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
void cpu6502::ADC() {
	
	//todo: add cycle if page boundary crossed

	uint8_t pre = a;
	a = a + data + reg.C;

	// C //
	if ((pre + data + reg.C) > 0xFF)
		reg.C = 1;
	else
		reg.C = 0;

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

	// V //
	// algorithm courtesy of Disch, http://forums.nesdev.com/viewtopic.php?f=3&t=6331 //
	if ((pre ^ a) & (data ^ a) & 0x80)
		reg.V = 1;
	else
		reg.V = 0;

	// N //
	if (a & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

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

void cpu6502::ASL() {
	if (addrMode == &cpu6502::IMP) {

		// C //
		if (a & (1 << 7))
			reg.C = 1;
		else
			reg.C = 0;

		a = a << 1;

		// N //
		if (a & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;
	}
	else {	

		//uint8_t* val = reinterpret_cast<uint8_t*>(0x10000 | addr);		// todo: alter as needed
		uint8_t val = data;

		// C //
		if (val & (1 << 7))
			reg.C = 1;
		else
			reg.C = 0;

		val = val << 1;

		// N //
		if (val & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;

		m->write(val, addr);
	}
	
	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;
}

void cpu6502::BCC() {
	// todo: cycle changes //
	if (reg.C == 0)
		pc = addr;
}

void cpu6502::BCS() {
	// todo: cycle changes //
	if (reg.C)
		pc = addr;
}

void cpu6502::BEQ() {
	// todo: cycle changes //
	if (reg.Z)
		pc = addr;
}

void cpu6502::BIT() {
	
	// Z //
	if (a & data)
		reg.Z = 0;
	else
		reg.Z = 1;

	// V //
	if (data & (1 << 6))
		reg.V = 1;
	else
		reg.V = 0;

	// N //
	if (data & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::BMI() {
	// todo: cycle changes //
	if (reg.N) 
		pc = addr;
}

void cpu6502::BNE() {
	// todo: cycle changes //
	if (reg.Z == 0)
		pc = addr;
}

void cpu6502::BPL() {
	// todo: cycle changes //
	if (reg.N == 0)
		pc = addr;
}

void cpu6502::BRK() {

}

void cpu6502::BVC() {
	// todo: cycle changes //
	if (reg.V == 0)
		pc = addr;
}

void cpu6502::BVS() {
	// todo: cycle changes //
	if (reg.V)
		pc = addr;
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
	else
		reg.C = 0;

	// Z //
	if (a == data)
		reg.Z = 1;
	else
		reg.Z = 0;

	// N //
	if ((a - data) & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::CPX() {

	// C //
	if (x >= data)
		reg.C = 1;
	else
		reg.C = 0;

	// Z //
	if (x == data)
		reg.Z = 1;
	else
		reg.Z = 0;

	// N //
	if ((x - data) & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::CPY() {

	// C //
	if (y >= data)
		reg.C = 1;
	else
		reg.C = 0;

	// Z //
	if (y == data)
		reg.Z = 1;
	else
		reg.Z = 0;

	// N //
	if ((y - data) & (1 << 7))
		reg.N = 1;
	else
		reg.N = 0;
}

void cpu6502::DEC() {
	//*(reinterpret_cast<uint8_t*>(0x10000 | addr)) = (data - 1);		// todo: alter as needed
	m->write(data - 1, addr);
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

void cpu6502::INC() {
	//*(reinterpret_cast<uint8_t*>(0x10000 | addr)) = (data + 1);		// todo: alter as needed
	m->write(data + 1, addr);
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

void cpu6502::JSR() {

	uint16_t tmp = pc - 1;
	//*(reinterpret_cast<uint8_t*>(0x10100 | sp)) = (tmp & 0xFF00) >> 8;		// todo: should be 0x0100 | sp
	m->write(((tmp & 0xFF00) >> 8), (0x0100 | sp));
	sp--;
	//*(reinterpret_cast<uint8_t*>(0x10100 | sp)) = (tmp & 0x00FF);			// todo: should be 0x0100 | sp
	m->write(((tmp & 0x00FF)), (0x0100 | sp));
	sp--;

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

void cpu6502::LSR() {
	if (addrMode == &cpu6502::IMP) {

		// C //
		if (a & (1 << 0))
			reg.C = 1;
		else
			reg.C = 0;

		a = a >> 1;

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
	else {
		//uint8_t* val = reinterpret_cast<uint8_t*>(0x10000 | addr);		// todo: alter as needed
		uint8_t val = data;

		// C //
		if (val & (1 << 0))
			reg.C = 1;
		else
			reg.C = 0;

		val = val >> 1;

		// Z //
		if (val & 0xFF)
			reg.Z = 0;
		else
			reg.Z = 1;

		// N //
		if (val & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;

		m->write(val, addr);
	}
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
	//*(reinterpret_cast<uint8_t*>(0x10100 | sp)) = a;		// todo: should be 0x0100 | sp
	m->write(a, (0x0100 | sp));
	sp--;
}

void cpu6502::PHP() {
	//*(reinterpret_cast<uint8_t*>(0x10100 | sp)) = reg.pReg;		// todo: should be 0x0100 | sp
	m->write(reg.pReg, (0x0100 | sp));
	sp--;
}

void cpu6502::PLA() {
	sp++;
	//a = *(reinterpret_cast<uint8_t*>(0x10100 | sp));		// todo: should be 0x0100 | sp
	a = m->read(0x0100 | sp);

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
	//reg.pReg = *(reinterpret_cast<uint8_t*>(0x10100 | sp));		// todo: should be 0x0100 | sp
	reg.pReg = m->read(0x0100 | sp);
}

void cpu6502::ROL() {
	
	unsigned int tmp = reg.C;
	if (addrMode == &cpu6502::IMP) {		
		
		// C //
		if (a & (1 << 7))
			reg.C = 1;
		else
			reg.C = 0;

		a = a << 1;
		if (tmp)
			a = a | 0x01;

		// N //
		if (a & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;
	}
	else {

		//uint8_t* val = reinterpret_cast<uint8_t*>(0x10000 | addr);		// todo: alter as needed
		uint8_t val = data;

		// C //
		if (val & (1 << 7))
			reg.C = 1;
		else
			reg.C = 0;

		val = val << 1;
		if (tmp)
			m->write((val | 0x01), addr);
			//*val = *val | 0x01;

		// N //
		if (val & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;
	}

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;
}

void cpu6502::ROR() {

	unsigned int tmp = reg.C;
	if (addrMode == &cpu6502::IMP) {

		// C //
		if (a & (1 << 0))
			reg.C = 1;
		else
			reg.C = 0;

		a = a >> 1;
		if (tmp)
			a = a | 0x80;

		// N //
		if (a & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;
	}
	else {
		//uint8_t* val = reinterpret_cast<uint8_t*>(0x10000 | addr);		// todo: alter as needed
		uint8_t val = data;

		// C //
		if (val & (1 << 0))
			reg.C = 1;
		else
			reg.C = 0;

		val = val >> 1;
		if (tmp)
			//*val = *val | 0x80;
			m->write((val | 0x80), addr);

		// N //
		if (val & (1 << 7))
			reg.N = 1;
		else
			reg.N = 0;

	}

	// Z //
	if (a & 0xFF)
		reg.Z = 0;
	else
		reg.Z = 1;

}

void cpu6502::RTI() {

}

void cpu6502::RTS() {
	uint16_t jmpAddr = 0x0000;
	sp++;
	//jmpAddr = *(reinterpret_cast<uint8_t*>(0x10100 | sp));		// todo: should be 0x0100 | sp
	jmpAddr = m->read(0x0100 | sp);
	sp++;
	//jmpAddr |= *(reinterpret_cast<uint8_t*>(0x10100 | sp)) << 8;
	jmpAddr |= m->read(0x0100 | sp) << 8;

	jmpAddr++;
	pc = jmpAddr;
}

void cpu6502::SBC() {

	//todo: add cycle if page boundary crossed

	// flip the data bits and call ADC, http://forums.nesdev.com/viewtopic.php?t=8703 //
	data = data ^ 0xFF;
	ADC();
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

void cpu6502::STA() {
	//*(reinterpret_cast<uint8_t*>(0x10000 | addr)) = a;
	m->write(a, addr);
}

void cpu6502::STX() {
	//*(reinterpret_cast<uint8_t*>(0x10000 | addr)) = x;
	m->write(x, addr);
}

void cpu6502::STY() {
	//*(reinterpret_cast<uint8_t*>(0x10000 | addr)) = y;
	m->write(y, addr);
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

