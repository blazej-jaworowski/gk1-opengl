#version 330 core

in vec4 gl_FragCoord;
in vec3 frag_pos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(frag_pos.z / 3, 0, 1.0, 1.0);
} 