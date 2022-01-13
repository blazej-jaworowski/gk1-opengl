#version 330 core

in vec4 gl_FragCoord;
in vec3 frag_pos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 object_color;

void main()
{
    vec3 ambient_light_color = vec3(1.0, 1.0, 1.0);
    float ambient_strength = 0.1;

    vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 light_pos = vec3(5.0, 0.0, 10.0);
    float light_strength = 1.0;

    vec3 ambient = ambient_light_color * object_color * ambient_strength;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    vec3 diffuse = light_color * max(dot(norm, light_dir), 0.0) * light_strength;

    // FragColor = vec4(norm, 1.0);
    // float v = length(light_pos - frag_pos);
    // FragColor = vec4(vec3(v), 1.0);
    FragColor = vec4(object_color * (diffuse + ambient), 1.0);
} 