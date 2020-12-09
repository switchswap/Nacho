#include <iostream>
#include <chrono>
#include <thread>

// SDL / OpenGL
#include <glad/glad.h>
#include <SDL.h>
// Dear ImGui
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
// Chip8
#include "core/chip8.h"
#include "opengl/shader.h"
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
SDL_Window *window;
SDL_Event event;
std::string glslVersion;
bool done = false;

// GL vars
// Todo: May not be needed
SDL_GLContext glContext;

bool initSDL() {
    int windowWidth = 1024;
    int windowHeight = 512;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE
    );

#ifdef __APPLE__
    // GL 3.2 Core + GLSL 150
    glslVersion = "#version 150";
    SDL_GL_SetAttribute( // required on Mac OS
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
        );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
    // GL 3.2 Core + GLSL 150
    glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
    // GL 3.0 + GLSL 130
    glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

    auto window_flags = (SDL_WindowFlags) (
            SDL_WINDOW_SHOWN
            | SDL_WINDOW_OPENGL
    );

    window = SDL_CreateWindow(
            "Nacho",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowWidth,
            windowHeight,
            window_flags
    );

    if (!window) {
        SDL_Log("Could not create window.: %s", SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    if (gladLoadGL() == 0) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return false;
    }
    glViewport(0, 0, windowWidth, windowHeight);

    return true;
}


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

void getInput() {
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                done = true;
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

int main() {
    // Init SDL and load ROM
    if (!(initROM() && initSDL())) {
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    // setup Dear ImGui style
    ImGui::StyleColorsDark();

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());

    bool demo = false;

    float vertices[] = {
            0.0f, 0.5f, // Vertex 1 (X, Y)
            0.5f, -0.5f, // Vertex 2 (X, Y)
            -0.5f, -0.5f  // Vertex 3 (X, Y)
    };

    // Create vertex array objects
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader vertShader = Shader("../src/opengl/shaders/screen.vert", GL_VERTEX_SHADER, "vertex");
    Shader fragShader = Shader("../src/opengl/shaders/screen.frag", GL_FRAGMENT_SHADER, "fragment");

    // Main loops
    while (!done) {
        // Emulate Cycle
        nacho.fetchOpcode();
        nacho.executeOpcode();
        nacho.updateTimers();

        // Keypad interrupt
        getInput();

//        if (nacho.drawFrame) {
//            for (int i = 0; i < 2048; i++) {
//                pixels[i] = (nacho.display[i] * 0x00FFFFFFu) | 0xFF000000;
//            }
//
//            // Update SDL texture
//            SDL_UpdateTexture(texture, nullptr, pixels, 64 * sizeof(Uint32));
//            // Clear screen and render
//            SDL_RenderClear(renderer);
//            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
//            SDL_RenderPresent(renderer);
//
//            nacho.drawFrame = false;
//
//        }
//        // Draw scene if needed
//        drawScene();

        // Create shader program to connect both shaders
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertShader.getShader());
        glAttachShader(shaderProgram, fragShader.getShader());

        // Explicitly specify which output is written to which framebuffer
        // 0 by default so not really necessary
        glBindFragDataLocation(shaderProgram, 0, "outColor");

        // Link the program
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        // Link between vertex data and attributes
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
        // Enable vertex array
        glEnableVertexAttribArray(posAttrib);

        // Draw stuff
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        bool x = true;
        if (x) {
//            ImGui::ShowDemoWindow(&x);
//            ImGui::Begin("Debugger", &x, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
//            ImGui::SetWindowSize(ImVec2(1024, 512), ImGuiCond_Always);

            // Draw Menubar
            ImGui::BeginMainMenuBar();
            if (ImGui::BeginMenu("Show")) {
                if (ImGui::MenuItem("Demo")) {
                    demo = !demo;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
//            ImGui::End();
        }
        if (demo) {
            ImGui::ShowDemoWindow(&demo);
        }
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        ImVec4 clear_color = ImVec4(((float) 137 / 255), ((float) 229 / 255), ((float) 148 / 255), 1.00f);
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //15 for 60fps
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}