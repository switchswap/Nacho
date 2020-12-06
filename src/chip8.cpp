#include <tic.h>
#include "chip8.h"

chip8::chip8() {
}

chip8::~chip8() {
}

// Execute machine language subroutine at address NNN
void op_0nnn() {
}

// Clear the screen
void op_00E0() {

}

void chip8::executeOpcode() {
    switch (opcode & 0xF000) { // bitwise AND with mask to get first digit
        case 0x0000:
            // Can be 0NNN, 0x00E0, or 0x00EE
            switch (opcode){
                case 0x00E0:
                    op_00E0();
                    break;
                case 0x00EE:
                    op_00EE();
                    break;
                default:
                    op_0nnn();
                    break;
            }
            break;
        case 0x1000:
            op_1nnn();
            break;
        case 0x2000:
            op_2nnn();
            break;
        case 0x3000:
            op_3xnn();
            break;
        case 0x4000:
            op_4xnn();
            break;
        case 0x5000:
            if((opcode & 0x000F) == 0x0000){ // Verify that last digit is 0
                op_5xy0();
            }
            break;
        case 0x6000:
            op_6xnn();
            break;
        case 0x7000:
            op_7xnn();
            break;
        case 0x8000:
            // Can be 0x8xy0, 0x8xy1, 0x8xy2, 0x8xy3, 0x8xy4, 0x8xy5, 0x8xy6, 0x8xy7, or 0x8xyE
            switch(opcode & 0x000F){
                case 0x0000:
                    op_8xy0();
                    break;
                case 0x0001:
                    op_8xy1();
                    break;
                case 0x0002:
                    op_8xy2();
                    break;
                case 0x0003:
                    op_8xy3();
                    break;
                case 0x0004:
                    op_8xy4();
                    break;
                case 0x0005:
                    op_8xy5();
                    break;
                case 0x0006:
                    op_8xy6();
                    break;
                case 0x0007:
                    op_8xy7();
                    break;
                case 0x000E:
                    op_8xyE();
                    break;
                default:break;
            }
            break;
        case 0x9000:
            if((opcode & 0x000F) == 0x0000){ // Verify that last digit is 0
                op_9xy0();
            }
            break;
        case 0xA000:
            op_Annn();
            break;
        case 0xB000:
            op_Bnnn();
            break;
        case 0xC000:
            op_Cxnn();
            break;
        case 0xD000:
            op_Dxyn();
            break;
        case 0xE000:
            // Can be 0xEx9E or 0xExA1
            switch (opcode & 0x0FF){
                case 0x009E:
                    op_Ex9E();
                    break;
                case 0x00A1:
                    op_ExA1();
                    break;
                default:break;
            }
            break;
        case 0xF000:
            // Can be 0xFx07, 0xFx0A, 0xFx15, 0xFx18, 0xFx1E, OxFx29, 0xFx33, 0xFx55, or 0xFx65
            switch (opcode & 0x0FF){
                case 0x0007:
                    op_Fx07();
                    break;
                case 0x000A:
                    op_Fx0A();
                    break;
                case 0x0015:
                    op_Fx15();
                    break;
                case 0x0018:
                    op_Fx18();
                    break;
                case 0x001E:
                    op_Fx1E();
                    break;
                case 0x0029:
                    op_Fx29();
                    break;
                case 0x0033:
                    op_Fx33();
                    break;
                case 0x0055:
                    op_Fx55();
                    break;
                case 0x0065:
                    op_Fx65();
                    break;
                default:break;
            }
            break;
        default:
            break;
	}
}

// Store number nn in register Vx
void chip8::op_6xnn() {
    V[((opcode & 0x0F00) >> 8)] = static_cast<uint8_t>(opcode & 0x00FF);
}

// Store value of register Vy in Vx
void chip8::op_8xy0() {
    V[((opcode & 0x0F00) >> 8)] = V[((opcode & 0x00F0) >> 4)];
}

// Add number nn to register Vx
void chip8::op_7xnn() {
    V[((opcode & 0x0F00) >> 8)] += opcode & 0x00FF;
}

// Add the value of register VY to register VX
// Set VF to 01 if a carry occurs
// Set VF to 00 if a carry does not occur
void chip8::op_8xy4() {
    // Check for carry
    if((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) > 0xFF){
        V[0xF] = 0x01;
    }
    else{
        V[0xF] = 0x00;
    }
    // Add values
    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
}

// Subtract the value of register VY from register VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void chip8::op_8xy5() {
    // Check for borrow
    if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]){ // If VY > VX
        V[0xF] = 0x00;
    }
    else{
        V[0xF] = 0x01;
    }

    // Subtract values
    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
}

// Set register VX to value of VY minus VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void chip8::op_8xy7() {
    // Check for borrow
    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){ // If VX > VY
        V[0xF] = 0x00;
    }
    else{
        V[0xF] = 0x01;
    }

    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
}

// Set register VX to (VX AND VY)
void chip8::op_8xy2() {
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
}

// Set register VX to (VX OR VY)
void chip8::op_8xy1() {
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
}

// Set register VX to (VX XOR VY)
void chip8::op_8xy3() {
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
}

// Store the value of register VY shifted right one bit in register VX
// Set register VF to the least significant bit prior to shift
void chip8::op_8xy6() {
    V[0xF] = V[opcode & 0x000F]; // Right most digit
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] >> 1;
}

// Store the value of register VY shifted left one bit in register VX
// Set register VF to the most significant bit prior to shift
void chip8::op_8xyE() {
    V[0xF] = V[opcode & 0xF000] >> 16; // Left most digit
    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] << 1;
}


