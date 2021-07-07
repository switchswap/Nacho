#ifndef NACHO_CHIP8_SCREEN_H
#define NACHO_CHIP8_SCREEN_H

#include <SDL.h>
#include <glad/glad.h>
#include "../opengl/screen.h"
#include "../opengl/shader.h"
#include "../core/chip8.h"

class Chip8Screen: public Screen {
public:
    explicit Chip8Screen(Chip8 &chip8);
    void update() override;
    void drawScreen();

private:
    Chip8 &chip8;
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

    SDL_Event event;
    // Vertex array/buffer and shader program objects
    GLuint vao, vbo, ebo, shaderProgram, texture;
    // Vertex and Fragment shader
    Shader vertShader, fragShader;
    GLubyte textureArray[32][64][4];

    void initShaders();
    void getInput();
    void genTexture();
};


#endif //NACHO_CHIP8_SCREEN_H
