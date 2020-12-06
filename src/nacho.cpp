#include <iostream>
#include <chrono>
#include <thread>
#include <SDL.h>
#include "chip8.h"

#undef main


// Chip8 vars
Chip8 nacho;

// Create 64 * 32 (2048) pixel buffer for draw cycle
uint32_t pixels[2048];

uint8_t keyMap[16] = {
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_4,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_r,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_f,
        SDLK_z,
        SDLK_x,
        SDLK_x,
        SDLK_v
};

// SDL vars
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Event event;

bool initSDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create window and video output
    window = SDL_CreateWindow("Nacho", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 512,
                              SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Could not create window.: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        SDL_Log("Could not create renderer.: %s", SDL_GetError());
        return false;
    }
    SDL_RenderSetLogicalSize(renderer, 1024, 512);

    // Create texture to store frame buffer
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if (!texture) {
        SDL_Log("Could not create texture.: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool initROM() {
    std::string romFile = R"("<file>")";

    std::cout << "Initializing..." << std::endl;
    std::cout << "ROM Dump Start" << std::endl;
    if (Chip8::printRom(romFile)) {
        std::cout << std::endl << "ROM Dump End" << std::endl;
    }
    else {
        return false;
    }

    std::cout << "Loading ROM..." << std::endl;
    if (nacho.loadRom(romFile)) {
        std::cout << "ROM Loaded!" << std::endl;
    }
    else {
        return false;
    }

    return true;
}

void drawScene() {
    // Draw scene if needed
    if (nacho.drawFrame) {
        for (int i = 0; i < 2048; i++) {
            pixels[i] = (nacho.display[i] * 0x00FFFFFFu) | 0xFF000000;
        }
        // Update SDL texture
        SDL_UpdateTexture(texture, nullptr, pixels, 64 * sizeof(Uint32));
        // Clear screen and render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
}

void getInput() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0);
            case SDL_KEYDOWN:
                for (int i = 0; i < 16; i++) {
                    if (event.key.keysym.sym == keyMap[i]) {
                        nacho.keypad[i] = 1;
                    }
                }
                break;
            case SDL_KEYUP:
                for (int i = 0; i < 16; i++) {
                    if (event.key.keysym.sym == keyMap[i]) {
                        nacho.keypad[i] = 0;
                    }
                }
                break;
            default:
                break;
        }
    }
}

void imgui_init() {
//    if(ImGui::BeginMainMenuBar())
//    {
//        if (ImGui::BeginMenu("File"))
//        {
//            if(ImGui::MenuItem("New"))
//            {
//                //Do something
//            }
//            ImGui::EndMenu();
//        }
//
//        ImGui::EndMainMenuBar();
//    }
}

int main() {
    // Init SDL and load ROM
    if (!(initROM() && initSDL())) {
        return 1;
    }

    // Main loops
    while (true) {
        // Emulate Cycle
        nacho.fetchOpcode();
        nacho.executeOpcode();
        nacho.updateTimers();

        // Keypad interrupt
        getInput();

        // Draw scene if needed
        drawScene();
        std::this_thread::sleep_for(std::chrono::milliseconds (3));
    }
    return 0;
}