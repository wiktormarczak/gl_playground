#version 460
layout (location = 0) in vec3 attribute_position;
layout (location = 1) in vec3 attribute_normal;
layout (location = 2) in vec2 attribute_texture;

out vec3 vertex_normal;
out vec2 vertex_texture;

uniform mat4 view, projection;

void main()
{
    gl_Position = projection * view * vec4(attribute_position.x, attribute_position.y, attribute_position.z, 1.0);
    vertex_normal = (transpose(inverse(view)) * vec4(attribute_normal, 0.0)).xyz;
    vertex_texture = attribute_texture;
}
