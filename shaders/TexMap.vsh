#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texUV;

uniform mat4 mtxWorldViewProj;

out vec2 textureCoords;

void main()
{
    gl_Position = mtxWorldViewProj * vec4(position, 1.0f);
    textureCoords = texUV;
}
