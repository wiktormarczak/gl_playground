#version 460

out vec4 fragment_color;

uniform vec4 uniform_color;

void main()
{
    fragment_color = uniform_color;
}
