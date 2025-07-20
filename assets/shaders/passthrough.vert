#version 410 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec4 vertexPosition;

invariant gl_Position;

void main()
{
    gl_Position = projection * view * model * vertexPosition;
}
