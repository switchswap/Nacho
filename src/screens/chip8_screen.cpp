#include "chip8_screen.h"

Chip8Screen::Chip8Screen(Chip8 &chip8): chip8(chip8) {
    initShaders();
}

void Chip8Screen::update() {
    // Emulate Cycle
    chip8.fetchOpcode();
    chip8.executeOpcode();
    chip8.updateTimers();

    // Handle keypad interrupt
    getInput();

    // Update display
    drawScreen();
}

void Chip8Screen::initShaders() {
    // Create vertex array objects
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer
    glGenBuffers(1, &vbo); // Generate 1 buffer

    vertShader = Shader("../src/opengl/shaders/screen.vert", GL_VERTEX_SHADER, "vertex");
    fragShader = Shader("../src/opengl/shaders/screen.frag", GL_FRAGMENT_SHADER, "fragment");

    // Create shader program to connect both shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader.getShader());
    glAttachShader(shaderProgram, fragShader.getShader());

    // Explicitly specify which output is written to which framebuffer
    // 0 by default so not really necessary
    glBindFragDataLocation(shaderProgram, 0, "outColor");

    // Link the program
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

void Chip8Screen::drawScreen() const {
    float vertices[] = {
            0.0f, 0.5f, // Vertex 1 (X, Y)
            0.5f, -0.5f, // Vertex 2 (X, Y)
            -0.5f, -0.5f  // Vertex 3 (X, Y)
    };

    // Draw stuff
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Link between vertex data and attributes
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Enable vertex array
    glEnableVertexAttribArray(posAttrib);

    // Draw stuff
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Chip8Screen::getInput() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                for (int i = 0; i < 16; i++) {
                    if (event.key.keysym.sym == keyMap[i]) {
                        chip8.keypad[i] = 1;
                    }
                }
                break;
            case SDL_KEYUP:
                for (int i = 0; i < 16; i++) {
                    if (event.key.keysym.sym == keyMap[i]) {
                        chip8.keypad[i] = 0;
                    }
                }
                break;
            default:
                break;
        }
    }
}
