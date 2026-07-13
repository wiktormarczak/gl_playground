#version 460
in vec3 vertex_normal;
in vec2 vertex_texture;

out vec4 fragment_color;

uniform sampler2D actual_texture;

void main()
{
    float brightness = vertex_normal.z;
    // fragment_color = vec4(brightness, brightness, brightness, 1.0);
    fragment_color = texture(actual_texture, vertex_texture);
}
