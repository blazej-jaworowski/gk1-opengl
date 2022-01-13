#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 frag_pos;
out vec3 normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    mat4 transformation = projection_matrix * view_matrix * model_matrix;
    frag_pos = vec3(model_matrix * vec4(aPos, 1.0));
    gl_Position = transformation * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(model_matrix))) * aNormal;
}