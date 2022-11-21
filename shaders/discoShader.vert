#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


out vec3 normalOutput;
out vec3 posOutput;
out vec3 phongOutput;

uniform mat4 model;
uniform mat4 viewProj;

void main()
{
    normalOutput = normal;
    phongOutput = mat3(transpose(inverse(model))) * normal;
    posOutput =  vec3(model * vec4(position, 1.0));

    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = viewProj * model * vec4(position, 1.0);
    
    sampleExtraOutput = 1.0f;
}  