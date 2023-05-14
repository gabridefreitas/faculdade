#version 400

uniform mat4 projection;

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = projection * vec4(position, 1.0);
}