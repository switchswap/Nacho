#include "chip8.h"

chip8::chip8() {
}


chip8::~chip8() {
}

void chip8::executeOpcode() {
	switch (opcode & 0xF000) {
	case 0x0000:
		//can be 0x00E0 or 0x00EE
		break;
	case 0x1000:
		break;
	case 0x2000:
		break;
	case 0x3000:
		break;
	case 0x4000:
		break;
	case 0x5000:
		break;
	case 0x6000:
		break;
	case 0x7000:
		break;
	case 0x8000:
		// can be 0x8xy1, 0x8xy2, 0x8xy3, 0x8xy4, 0x8xy5, 0x8xy6, 0x8xy7, 0x8xyE
		break;
	case 0x9000:
		break;
	case 0xA000:
		break;
	case 0xB000:
		break;
	case 0xC000:
		break;
	case 0xD000:
		break;
	case 0xE000:
		break;
	case 0xF000:
		break;
	}
}

void op_1nnn() {

}

void op_2nnn() {

}

void op_3xkk() {

}

void op_4xkk() {

}

void op_5xy0() {

}

void op_6xkk() {

}

void op_7xkk() {

}

void op_9xy0() {

}

void op_Annn() {

}

void op_Bnnn() {

}

void op_Cxkk() {

}

void op_Dxyn() {

}

//todo 0, 8, E, F opcodes