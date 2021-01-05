#version 330
uniform float uR; //球半径
in vec3 vPosition;
out vec4 fragColor;

void main(void)
{
    fragColor = vec4(0.6, 0, 0, 0);
}
