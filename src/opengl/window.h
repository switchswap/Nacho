#ifndef NACHO_WINDOW_H
#define NACHO_WINDOW_H

#include <SDL.h>
#include <glad/glad.h>
#include <imgui_impl_sdl.h>
#include <string>
#include <vector>
#include "screen.h"

class Window {
public:
    Window(const std::string& title, const int width, const int height);
    void setClearColor(ImVec4 color);
    void setClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void startLoop();
    void addScreen(Screen* screen);
    void close();

    SDL_Window *window;
    SDL_GLContext glContext;

private:
    std::string title;
    std::vector<Screen*> screenList;
    std::string glslVersion;
    bool shouldClose = false;
};
#endif //NACHO_WINDOW_H
