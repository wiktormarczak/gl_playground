#version 460
layout (location = 0) in vec2 attribute_position;
layout (location = 1) in uint attribute_color_index;

out uint vertex_color_index;

void main()
{
    gl_Position = vec4(attribute_position.x, attribute_position.y, 0.0, 1.0);
    vertex_color_index = attribute_color_index;
}
