#version 430 core

layout (location = 0) in vec4 vertex_color;

layout (location = 0) out vec4 frag_color;



void main() 
{
    frag_color = vertex_color;
}
