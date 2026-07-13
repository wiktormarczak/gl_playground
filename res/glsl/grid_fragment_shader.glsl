#version 460
in vec3 vertex_position;

out vec4 fragment_color;

uniform vec3 camera_position;

void main()
{
    float d = distance(vertex_position, camera_position);
    fragment_color = vec4(1.0, 1.0, 1.0, 19.0 - d);
}
