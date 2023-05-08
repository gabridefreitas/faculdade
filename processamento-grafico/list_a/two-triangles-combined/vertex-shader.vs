#version 400

uniform vec3 vertex_color;

layout (location = 0) in vec3 position;

out vec4 finalColor;

void main()
{
    gl_Position = vec4(position, 1.0);

    finalColor = vec4(vertex_color, 1.0);
}