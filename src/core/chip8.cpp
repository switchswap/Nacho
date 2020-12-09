#include "chip8.h"
#include <iostream>
#include <iomanip>

#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << (int)(x)

uint8_t fontSet[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    // Clear out RAM
    for (uint8_t &i : RAM) {
        i = 0;
    }

    // Clear display
    for (uint8_t &i : display) {
        i = 0;
    }

    // Clear registers, stack, and keypad
    for (int i = 0; i < 16; i++) {
        stack[i] = 0;
        keypad[i] = 0;
        V[i] = 0;
    }

    // Clear variables
    I = 0;
    PC = 0x200;
    SP = 0;
    opcode = 0;

    // Load font set into memory
    for (int i = 0; i < 80; i++) {
        RAM[i] = fontSet[i];
    }

    // Reset timers
    delayTimer = 0;
    soundTimer = 0;
}

Chip8::~Chip8() = default;



bool Chip8::printRom(const std::string &fileName) {
    std::ifstream romFile(fileName, std::ios::in | std::ios::binary);
    if (!romFile) {
        std::cerr << "Cannot open file!" << std::endl;
        return false;
    }

    char buffer[1];
    while (romFile.read(buffer, 1)) {
        std::cout << HEX((uint8_t)buffer[0]) << " ";
    }
    return true;
}

/**
 * Load rom into memory (Starting at address 200 (512 dec))
 */
bool Chip8::loadRom(const std::string &fileName) {
    int count = 0;
    std::ifstream romFile(fileName, std::ios::in | std::ios::binary);
    if (!romFile) {
        std::cerr << "Cannot open file!" << std::endl;
        return false;
    }

    char buffer[1];
    while (romFile.read(buffer, 1)) {
        RAM[512 + count] = (uint8_t)buffer[0];
        count++;
    }
    romFile.close();
    return true;
}

void Chip8::executeOpcode() {
    switch (opcode & 0xF000) { // Bitwise AND with mask to get first digit
        case 0x0000:
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
            std::cout << "Opcode: " << opcode << " not implemented." << std::endl;
            break;
    }
}

// Execute machine language subroutine at address NNN
void Chip8::op_0nnn() {
    // Won't implement at the moment
}

// Clear the display
void Chip8::op_00E0() {
    // Set each pixel value to 0
    for(uint8_t &i : display){
        i = 0;
    }
}

// Subtract 1 from the stack pointer
// Set program counter to address at top of stack
void Chip8::op_00EE() {
    SP--;
    PC = stack[SP];
}

// Set program counter to NNN
void Chip8::op_1nnn() {
    PC = get_NNN();
}

// Call subroutine at NNN
void Chip8::op_2nnn() {
    // Add current PC to stop of stack
    stack[SP] = PC;

    // Increment stack pointer
    SP++;

    // Set PC to nnn
    PC = get_NNN();

}

// Skip next instruction if VX == NN.
void Chip8::op_3xnn() {
    if(V[get_x()] == get_NN()) {
        // Increment program counter by 2 so to skip the next instruction
        PC += 2;
    }
}

// Skip next instruction if VX != NN.
void Chip8::op_4xnn() {
    if(V[get_x()] != get_NN()) {
        // Increment program counter by 2 so to skip the next instruction
        PC += 2;
    }
}

//  Skip next instruction if Vx == Vy.
void Chip8::op_5xy0() {
    if(V[get_x()] == V[get_y()]) {
        // Increment program counter by 2 so to skip the next instruction
        PC +=2;
    }
}

// Store number nn in register Vx
void Chip8::op_6xnn() {
    V[get_x()] = static_cast<uint8_t>(get_NN());
}

// Store value of register Vy in Vx
void Chip8::op_8xy0() {
    V[get_x()] = V[get_y()];
}

// Add number nn to register Vx
void Chip8::op_7xnn() {
    V[get_x()] += get_NN();
}

// Add the value of register VY to register VX
// Set VF to 01 if a carry occurs
// Set VF to 00 if a carry does not occur
void Chip8::op_8xy4() {
    // Check for carry
    int vx = V[get_x()];
    int vy = V[get_y()];

    if (vx + vy > 0xFF) {
        V[0xF] = 0x01;
    }
    else{
        V[0xF] = 0x00;
    }

    // Add values
    V[get_x()] += V[get_y()];
}

// Subtract the value of register VY from register VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void Chip8::op_8xy5() {
    // Check for borrow
    int vx = V[get_x()];
    int vy = V[get_y()];

    if(vy > vx){ // If VY > VX
        V[0xF] = 0x00;
    }
    else{
        V[0xF] = 0x01;
    }

    // Subtract values
    V[get_x()] -= V[get_y()];
}

// Set register VX to (VX AND VY)
void Chip8::op_8xy2() {
    V[get_x()] = V[get_x()] & V[get_y()];
}

// Set register VX to (VX OR VY)
void Chip8::op_8xy1() {
    V[get_x()] = V[get_x()] | V[get_y()];
}

// Set register VX to (VX XOR VY)
void Chip8::op_8xy3() {
    V[get_x()] = V[get_x()] ^ V[get_y()];
}

// Shift VX one bit to the right
// Set VF to the value of the shifted out bit
void Chip8::op_8xy6() {
    V[0xF] = V[get_x()] & 0x1u; // Right most digit
    V[get_x()] >>= 1u;
}

// Set register VX to value of VY minus VX
// Set VF to 00 if a borrow occurs
// Set VF to 01 if a borrow does not occur
void Chip8::op_8xy7() {
    // Check for borrow
    if(V[get_x()] > V[get_y()]){ // If VX > VY
        V[0xF] = 0x00;
    }
    else{
        V[0xF] = 0x01;
    }

    V[get_x()] = V[get_y()] - V[get_x()];
}

// Shift VX one bit to the left
// Set VF to the value of the shifted out bit
void Chip8::op_8xyE() {
    V[0xF] = V[get_x()] >> 7u; // Left most digit of VX
    V[get_x()] <<= 1u; // Shift value of VX one bit to left
}

//  Skip next instruction if VX != VY.
void Chip8::op_9xy0() {
    if(V[get_x()] != V[get_y()]) {
        // Increment program counter by 2 so to skip the next instruction
        PC +=2;
    }
}

// The value of register I is set to NNN.
void Chip8::op_Annn() {
    I = get_NNN();
}

// Jump to location nnn + V0.
void Chip8::op_Bnnn() {
    PC = get_NNN() + V[0];
}

// Set Vx to a random byte ANDed with nn
void Chip8::op_Cxnn() {
    uint8_t random = rand() % 0xFF;
    V[get_x()] = random & get_NN();
}

void Chip8::op_Dxyn() {
    int x = V[get_x()] % 64;
    int y = V[get_y()] % 32;

    V[0xF] = 0;
    uint8_t sprite;
    // For n rows, write the data to the display
    for (int yIndex = 0; yIndex < get_N(); yIndex++) {
        // If y will go past window size, break
        if ((y + yIndex) >= 32) {
            break;
        }

        sprite = RAM[I + yIndex];
        // For each bit in sprite on this line, write to display if needed
        for (int xIndex = 0; xIndex < 8; xIndex++) {
            if ((x + xIndex) >= 64) {
                break;
            }

            // If bit is not 0, potentially toggle display pixel
            if (sprite & (0x80 >> xIndex)) {
                if (display[(64 * (y + yIndex)) + x + xIndex]) {
                    V[0xF] = 1;
                }
                display[(64 * (y + yIndex)) + x + xIndex] ^= 1u;
            }
        }
    }
    drawFrame = true;
}

// Skip one instruction if key corresponding to value in Vx is pressed
void Chip8::op_Ex9E() {
    if (keypad[V[get_x()]]) {
        PC += 2;
    }
}

// Skip one instruction if key corresponding to value in Vx is NOT pressed
void Chip8::op_ExA1() {
    if (!keypad[V[get_x()]]) {
        PC += 2;
    }
}

// Set Vx to value of delay timer
void Chip8::op_Fx07() {
    V[get_x()] = delayTimer;
}

// Stops execution and waits for key input
// If key pressed, hex value of key is placed in VX
// Decrements PC if no key pressed
void Chip8::op_Fx0A() {
    bool pressed = false;
    for (int i = 0; i < 16; i++) {
        if (keypad[i]) {
            V[get_x()] = i;
            pressed = true;
        }
    }

    if (!pressed) {
        PC -= 2;
    }
}

// Set the delay timer to the value in VX
void Chip8::op_Fx15() {
    delayTimer = V[get_x()];
}

// FX18 sets the sound timer to the value in VX
void Chip8::op_Fx18() {
    soundTimer = V[get_x()];
}

// Add value in Vx to I and set VF overflow flag (doesn't necessarily need to set flag but will do anyways)
void Chip8::op_Fx1E() {
    int temp = I + V[get_x()];
    I += V[get_x()];
    if (temp > 4096) {
        V[15] = 1;
    }
}

// I is set to the address of the hex character in VX
void Chip8::op_Fx29() {
    I = V[get_x()] * 5;
}

// Takes number in VX and converts it to 3 decimal digits and stores that in I
void Chip8::op_Fx33() {
    uint8_t num = V[get_x()];
    RAM[I] = (uint8_t)(num / 100);
    RAM[I + 1] = (uint8_t)((num / 10) % 10);
    RAM[I + 2] = (uint8_t)((num % 100) % 10);
}

// Store registers V0 -> VX to memory starting at address I
void Chip8::op_Fx55() {
    for (int i = 0; i <= get_x(); i++) {
        RAM[I + i] = V[i];
    }
}


// Load registers from memory to V0 -> VX
void Chip8::op_Fx65() {
    for (int i = 0; i <= get_x(); i++) {
        V[i] = RAM[I + i];
    }
}

uint16_t Chip8::fetchOpcode() {
    // Convert two bytes from the program counter into one uint16_t
    uint16_t temp = RAM[PC];
    temp = temp << 8u;
    temp += RAM[PC + 1];

    // Update opcode
    opcode = temp;

    // Up program counter by two since we've read two bytes
    PC += 2;

    // Return opcode
    return temp;
}

uint8_t Chip8::get_x() const {
    return (opcode & 0x0F00u) >> 8u;
}

uint8_t Chip8::get_y() const {
    return (opcode & 0x00F0u) >> 4u;
}

uint8_t Chip8::get_N() const {
    return opcode & 0x000Fu;
}

uint8_t Chip8::get_NN() const {
    return opcode & 0x00FFu;
}

uint16_t Chip8::get_NNN() const {
    return opcode & 0x0FFFu;
}

void Chip8::updateTimers() {
    if (delayTimer > 0) {
        delayTimer--;
    }

    if (soundTimer > 0) {
        soundTimer--;
    }
}
