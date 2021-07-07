#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 inTexCoord;

out vec4 vertexColor; // Specify a color output to the fragment shader
out vec2 texCoord;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 0.0); // This color is red
    texCoord = inTexCoord;
}