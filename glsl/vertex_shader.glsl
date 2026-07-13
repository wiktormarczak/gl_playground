#version 460
layout (location = 0) in vec3 attribute_position;
layout (location = 1) in vec3 attribute_normal;

out vec3 vertex_normal;

uniform mat4 view, projection;

void main()
{
    gl_Position = projection * view * vec4(attribute_position.x, attribute_position.y, attribute_position.z, 1.0);
    vertex_normal = attribute_normal;
}
