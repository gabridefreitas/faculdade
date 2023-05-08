#version 400

uniform vec3 vertex_color;
uniform float ratio;
uniform mat4 rotate;
uniform bool shouldRotate;
uniform mat4 translate;
uniform bool shouldTranslate;

layout (location = 0) in vec3 position;

out vec4 finalColor;

void main()
{
    if (shouldRotate)
        gl_Position = rotate * ratio * vec4(position, 1.0);
    else if (shouldTranslate)
        gl_Position = translate * ratio * vec4(position, 1.0);
    else
        gl_Position = ratio * vec4(position, 1.0);

    finalColor = vec4(vertex_color, 1.0);
}