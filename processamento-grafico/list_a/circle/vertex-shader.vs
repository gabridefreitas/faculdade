#version 400

layout (location = 0) in vec3 position;

uniform float ratio;

void main()
{
    gl_Position = ratio * vec4(position, 1.0);
}