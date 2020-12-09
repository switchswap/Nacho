#ifndef NACHO_SHADER_H
#define NACHO_SHADER_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    GLuint type;

    Shader() {}
    explicit Shader(const std::string &path, GLuint type, const std::string tag) {
        this->type = type;
        this->tag = tag;
        loadShader(path);
        compileShader();
        checkCompilationErrors();
    }

    GLuint getShader() {
        return shader;
    }

private:
    std::string shaderString;
    std::string tag;
    GLuint shader;

    void loadShader(const std::string &shaderPath) {
        // Open file as input
        std::ifstream shaderFile(shaderPath, std::ios::in);

        // Error check
        if (!shaderFile) {
            std::cerr << "Cannot open file!" << std::endl;
            shaderString = "";
            return;
        }

        // Load file contents into a stringstream
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        // Load stringstream into string and close file
        shaderString = shaderStream.str();
        shaderFile.close();
    }

    void compileShader() {
        // Create vertex shader
        shader = glCreateShader(type);
        const char* shaderSource = shaderString.c_str();
        // Load data into shader
        glShaderSource(shader, 1, &shaderSource, NULL);
        // Compile shader for graphics card to understand
        glCompileShader(shader);
    }

    bool checkCompilationErrors() {
        // Check for compilation errors
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            fprintf(stderr, "Failed to compile %s shader!\n", tag.c_str());
            char buffer[512];
            glGetShaderInfoLog(shader, 512, NULL, buffer);
            fprintf(stderr, "%s", buffer);
            return true;
        }
        return false;
    }
};


#endif //NACHO_SHADER_H
