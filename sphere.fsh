#version 330
uniform float uR; //球半径
in vec3 vPosition;
in vec4 vAmbient;
in vec4 vDiffuse;
in vec4 vSpecular;
out vec4 fragColor;
uniform vec4 uColor;

void main(void)
{
    vec4 finalColor = vec4(0.6, 0, 0, 0);
    finalColor = uColor * (vAmbient + vDiffuse + vSpecular);
    fragColor = finalColor;
}
