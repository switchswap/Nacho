#pragma once
#include <cstdint>
#include <cstdlib>
#include <fstream>

class Chip8 {
public:
    Chip8();
    ~Chip8();
    uint16_t fetchOpcode();
    void executeOpcode();
    bool loadRom(const std::string& fileName);
    static bool printRom(const std::string& fileName);
    uint8_t display[64 * 32]{}; // 64 x 32 (4096) screen size
    uint8_t keypad[16]{}; // 0x0 to 0xF (16 buttons)
    bool drawFrame{};
    void updateTimers();

private:
    uint8_t RAM[0x1000]{}; // 0x000 to 0xFFF (4kb RAM)
    uint8_t V[0x10]{}; // Registers 0x0 to 0xF (16 registers)
    uint16_t I; // Index register
    uint16_t PC; // Program Counter
    uint16_t stack[0x10]{};
    uint8_t SP; // Stack pointer (Points to top of stack)
    uint16_t opcode; // 2 byte opcode
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t get_x() const;
    uint8_t get_y() const;
    uint8_t get_N() const;
    uint8_t get_NN() const;
    uint16_t get_NNN() const;

    void op_0nnn();
    void op_00E0();
    void op_00EE();
    void op_1nnn();
    void op_2nnn();
    void op_3xnn();
    void op_4xnn();
    void op_5xy0();
    void op_6xnn();
    void op_7xnn();
    void op_8xy0();
    void op_8xy1();
    void op_8xy2();
    void op_8xy3();
    void op_8xy4();
    void op_8xy5();
    void op_8xy6();
    void op_8xy7();
    void op_8xyE();
    void op_9xy0();
    void op_Annn();
    void op_Bnnn();
    void op_Cxnn();
    void op_Dxyn();
    void op_Ex9E();
    void op_ExA1();
    void op_Fx07();
    void op_Fx0A();
    void op_Fx15();
    void op_Fx18();
    void op_Fx1E();
    void op_Fx29();
    void op_Fx33();
    void op_Fx55();
    void op_Fx65();
};