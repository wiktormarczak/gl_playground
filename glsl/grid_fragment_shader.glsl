#version 460
out vec4 fragment_color;

void main()
{
    if(mod(gl_FragCoord.x, 100.0f) > 1.0f)
        discard;

    fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
