#version 460
uniform mat4 view, projection;
uniform uint count;
uniform float spacing;
uniform vec3 camera_position;

out vec3 vertex_position;

void main()
{
    const float bound = ((count - 1) * spacing) / 2;
    vec4 position;

    if(gl_VertexID < 2 * count)
    {
        const uint index = gl_VertexID;
        position.x = (index / 2) * spacing - bound;
        position.y = 0.0;
        position.z = (2 * mod(index, 2) - 1) * bound;
        position.w = 1.0;
    }
    else
    {
        const uint index = gl_VertexID - 2 * count;
        position.x = (2 * mod(index, 2) - 1) * bound;
        position.y = 0.0;
        position.z = (index / 2) * spacing - bound;
        position.w = 1.0;
    }

    position.x += floor(camera_position.x / spacing) * spacing;
    position.z += floor(camera_position.z / spacing) * spacing;

    gl_Position = projection * view * position;
    vertex_position = position.xyz;
}
