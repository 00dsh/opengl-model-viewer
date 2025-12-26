#version 400 core

layout (location = 0) in vec4 vertexPos;
layout (location = 1) in vec2 texturePos;

// # To fragment shader
out float lightVal;
out vec2 texPos;

uniform mat4 finalMat;

void main()
{
    gl_Position = finalMat * vec4(vertexPos.xyz, 1.0f);
    lightVal = vertexPos.w;
    texPos = texturePos;
}