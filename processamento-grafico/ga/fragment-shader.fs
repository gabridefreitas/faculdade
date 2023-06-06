#version 400

uniform sampler2D tex_buffer;

in vec4 finalColor;
in vec2 texturePosition;

out vec4 color;

void main()
{
    // color = finalColor;
    color = texture(tex_buffer, texturePosition);
}