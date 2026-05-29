#version 460
flat in uint vertex_color_index;

out vec4 fragment_color;

void main()
{
    switch(vertex_color_index)
    {
        case 0u:
            fragment_color = vec4(1.0, 0.0, 0.0, 1.0);
            break;
        case 1u:
            fragment_color = vec4(0.0, 1.0, 0.0, 1.0);
            break;
        case 2u:
            fragment_color = vec4(0.0, 0.0, 1.0, 1.0);
            break;
        default:
            fragment_color = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
