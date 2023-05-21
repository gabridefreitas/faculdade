#version 400

uniform mat4 projection;
uniform mat4 model;
uniform vec3 color;

layout (location = 0) in vec3 position;

out vec3 fragColor;

void main()
{
    gl_Position =  projection * model * vec4(position, 1.0);

    fragColor = color;
}