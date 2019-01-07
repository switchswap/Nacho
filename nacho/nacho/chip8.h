#pragma once
class chip8 {
public:
	chip8();
	~chip8();
	void executeOpcode();
protected:
	char RAM[0x1000]; //0x000 to 0xFFF
	char V[0x10]; //Registers 0x0 to 0xF
	short I;
	short PC; // Program Counter 
	short stack[0x10];
	char SP; // Stack pointer
	char delayTimer;
	char soundTimer;
	char keypad[16]; //0x0 to 0xF
	short opcode;
	char graphics[64 * 32];
};

