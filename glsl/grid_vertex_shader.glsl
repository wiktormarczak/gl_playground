#version 460
layout (location = 0) in vec3 attribute_position;

out vec3 vertex_position;

uniform mat4 uniform_matrix;

void main()
{
    gl_Position = uniform_matrix * vec4(attribute_position, 1.0);
    vertex_position = attribute_position;
}
