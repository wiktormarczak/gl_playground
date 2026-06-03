#version 460
layout (location = 0) in vec3 attribute_position;

out vec3 vertex_position;

uniform mat4 view, projection;

void main()
{
    gl_Position = projection * view * vec4(attribute_position, 1.0);
    vertex_position = attribute_position;
}
