#include <iostream>

// SDL / OpenGL
#include <glad/glad.h>
#include <SDL.h>
// Dear ImGui
#include <imgui_impl_sdl.h>
// Chip8
#include "core/chip8.h"
#include "opengl/shader.h"
#include "opengl/window.h"
#include "screens/chip8_screen.h"
#include "screens/debug_screen.h"

#undef main


// Chip8 vars
Chip8 nacho;

// Create 64 * 32 (2048) pixel buffer for draw cycle
uint32_t pixels[2048];

bool initROM() {
    std::string romFile = R"(../roms/pong.ch8)";

    std::cout << "Initializing..." << std::endl;
    std::cout << "ROM Dump Start" << std::endl;
    if (Chip8::printRom(romFile)) {
        std::cout << std::endl << "ROM Dump End" << std::endl;
    } else {
        return false;
    }

    std::cout << "Loading ROM..." << std::endl;
    if (nacho.loadRom(romFile)) {
        std::cout << "ROM Loaded!" << std::endl;
    } else {
        return false;
    }

    return true;
}

//void drawScene() {
//    // Draw scene if needed
//    if (nacho.drawFrame) {
//        for (int i = 0; i < 2048; i++) {
//            pixels[i] = (nacho.display[i] * 0x00FFFFFFu) | 0xFF000000;
//        }
//        // Update SDL texture
//        SDL_UpdateTexture(texture, nullptr, pixels, 64 * sizeof(Uint32));
//        // Clear screen and render
//        SDL_RenderClear(renderer);
//        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
//        SDL_RenderPresent(renderer);
//
//        nacho.drawFrame = false;
//    }
//}


int main() {
    if (!initROM()) {
        return 1;
    }

    ImVec4 clear_color = ImVec4(((float) 137 / 255), ((float) 229 / 255), ((float) 148 / 255), 1.00f);
    Window window1("Nacho", 1024, 512);
    window1.setClearColor(clear_color);

    Chip8Screen chip8Screen(nacho);
    DebugScreen debugScreen(nacho);

    window1.addScreen(&chip8Screen);
    window1.addScreen(&debugScreen);
    window1.startLoop();
    return 0;
}