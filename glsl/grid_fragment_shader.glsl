#version 460
in vec3 vertex_position;

out vec4 fragment_color;

void main()
{
    if(mod(vertex_position.x, 1.0) > 0.01 && mod(vertex_position.z, 1.0) > 0.01)
        discard;

    fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
