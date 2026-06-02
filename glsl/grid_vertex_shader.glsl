#version 460
layout (location = 0) in vec3 attribute_position;

uniform mat4 uniform_matrix;

void main()
{
    gl_Position = uniform_matrix * vec4(attribute_position, 1.0);
}
