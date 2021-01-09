#version 430 core
uniform mat4 uPMatrix,uVMatrix;
layout (location = 0) in vec3 aPosition;

void main()
{
    gl_Position = uPMatrix * uVMatrix * vec4(aPosition,1);
}
