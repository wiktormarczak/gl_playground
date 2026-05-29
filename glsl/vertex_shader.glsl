#version 460
layout (location = 0) in vec3 attribute_position;
layout (location = 1) in vec3 attribute_color;

out vec3 vertex_color;

void main()
{
    gl_Position = vec4(attribute_position.x, attribute_position.y, attribute_position.z, 1.0);
    vertex_color = attribute_color;
}
