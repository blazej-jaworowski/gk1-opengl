#version 330 core

in vec4 gl_FragCoord;
in vec3 frag_pos;
in vec3 normal;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

#define DIR_LIGHT_COUNT 2

uniform Material material;
uniform DirLight dir_lights[DIR_LIGHT_COUNT];

uniform vec3 camera_pos;
uniform bool blinn_reflection;

vec3 get_dir_light(vec3 norm) {
    vec3 sum = vec3(0);
    for(int i = 0; i < DIR_LIGHT_COUNT; i++) {
        vec3 ambient = dir_lights[i].ambient * material.ambient;
        vec3 diffuse = max(dot(-dir_lights[i].direction, norm), 0.0)
                     * dir_lights[i].diffuse * material.diffuse;

        vec3 view_dir = normalize(camera_pos - frag_pos);
        vec3 specular;
        if(blinn_reflection) {
            vec3 half_dir = normalize(dir_lights[i].direction + view_dir);
            float spec_angle = max(dot(half_dir, norm), 0.0);
            specular = pow(spec_angle, material.shininess * 4)
                       * dir_lights[i].specular * material.specular;
        } else {
            vec3 reflect_dir = reflect(-dir_lights[i].direction, norm);
            float spec_angle = max(dot(view_dir, reflect_dir), 0.0);
            specular = pow(spec_angle, material.shininess)
                        * dir_lights[i].specular * material.specular;
        }
        sum += ambient + diffuse + specular;
    }
    return sum;
};


void main()
{
    vec3 norm = normalize(normal);
    vec3 color = get_dir_light(norm);
    FragColor = vec4(color, 1.0);
} 