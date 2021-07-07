#version 330 core

out vec4 outColor;
in vec4 vertexColor; // Input from vertex shader
in vec2 texCoord;

uniform sampler2D myTexture;

void main() {
    outColor = texture(myTexture, texCoord);
}
