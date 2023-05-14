#version 400

uniform float ratio;

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = ratio * vec4(position, 1.0);
}