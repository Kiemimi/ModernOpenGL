#version 330 core
out vec4 FragColor;

uniform vec3 emissive;

void main()
{
    FragColor = vec4(normalize(emissive), 1.0f); // set alle 4 vector values to 1.0
}