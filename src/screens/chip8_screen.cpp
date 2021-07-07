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
    glGenBuffers(1, &vbo); // Generate 1 vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Create element buffer
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

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

    // Draw using wireframe mode
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void Chip8Screen::genTexture() {
    int i, j, c;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < 64; j++) {
            c = chip8.display[i*64 + j]*255;
            textureArray[i][j][0] = (GLubyte) c;
            textureArray[i][j][1] = (GLubyte) c;
            textureArray[i][j][2] = (GLubyte) c;
            textureArray[i][j][3] = (GLubyte) 255;
        }
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureArray);
}

void Chip8Screen::drawScreen() {
    float vertices[] = {
            // Pos          // Texture coords
             1.0f,  1.0f,   1.0f, 0.0f, // Vertex 0 (X, Y) Top Right
             1.0f, -1.0f,   1.0f, 1.0f,  // Vertex 1 (X, Y) Bottom Right
            -1.0f, -1.0f,   0.0f, 1.0f, // Vertex 2 (X, Y) Bottom Left
            -1.0f,  1.0f,   0.0f, 0.0f // Vertex 3 (X, Y) Top Left
    };
    unsigned int indices[] = {
            0, 1, 3, // Triangle 1
            1, 2, 3  // Triangle 2
    };

    // Copy vertices array into vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    // Copy index array into element bugger
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    // Link between vertex data and attributes
    // Position input
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    // Texture coord input
    GLint texAttrib = glGetAttribLocation(shaderProgram, "inTexCoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Enable vertex array
    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(texAttrib);

    genTexture();

    // Draw stuff
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
