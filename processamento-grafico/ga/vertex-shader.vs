#version 400

uniform mat4 projection;
uniform mat4 model;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 fragColor;
layout (location = 2) in vec2 textureCoordinates;

out vec4 finalColor;
out vec2 texturePosition;

void main()
{
    gl_Position =  projection * model * vec4(position, 1.0);

    finalColor = vec4(fragColor, 1.0);
    texturePosition = vec2(textureCoordinates.x, 1 - textureCoordinates.y);
}