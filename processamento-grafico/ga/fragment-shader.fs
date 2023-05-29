#version 400

uniform sampler2D tex_buffer;

in vec2 texturePosition;

out vec4 color;

void main()
{
    // color = texture(tex_buffer, texturePosition);
    color = vec4(1.0, 0.0, 0.0, 1.0);
}