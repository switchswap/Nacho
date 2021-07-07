#include "window.h"
#include <chrono>
#include <thread>
#include <imgui_impl_opengl3.h>

Window::Window(const std::string& title, const int width, const int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        fprintf(stderr, "Unable to initialize SDL: %s", SDL_GetError());
    }

    // Set OpenGL attributes
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
    // GL 3.3 + GLSL 330
    glslVersion = "#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

    auto window_flags = (SDL_WindowFlags) (
            SDL_WINDOW_SHOWN
            | SDL_WINDOW_OPENGL
    );

    // Create window
    window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            window_flags
    );

    if (!window) {
        SDL_Log("Could not create window.: %s", SDL_GetError());
        fprintf(stderr, "Could not create window.: %s", SDL_GetError());
    }

    // Initialize OpenGL context
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    if (gladLoadGL() == 0) {
        fprintf(stderr, "Failed to initialize OpenGL context!\n");
    }
    glViewport(0, 0, width, height - 18); // Subtract height of imgui bar

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

void Window::startLoop() {
    while (!shouldClose) {
        getInput();

        // Clear screen to start drawing
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // Update all the screens
        for (Screen *screen: screenList) {
            screen->update();
        }

        // Update display
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); //15 for 60fps
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::setClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    glClearColor(red, green, blue, alpha);
}

void Window::setClearColor(ImVec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void Window::addScreen(Screen *screen) {
    screenList.push_back(screen);
}


void Window::getInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                shouldClose = true;
            default:
                break;
        }
    }
}