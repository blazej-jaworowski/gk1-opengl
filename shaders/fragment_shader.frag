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

struct SunLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

uniform Material material;
uniform SunLight sun_light;

uniform vec3 camera_pos;

vec3 get_sun_light(vec3 norm) {
    vec3 ambient = sun_light.ambient * material.ambient;
    vec3 diffuse = max(dot(-sun_light.direction, norm), 0.0)
                 * sun_light.diffuse * material.diffuse;
    vec3 view_dir = normalize(camera_pos - frag_pos);
    vec3 reflect_dir = reflect(-sun_light.direction, norm);
    vec3 specular = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)
                    * sun_light.specular * material.specular;

    return ambient + diffuse + specular;
};

void main()
{
    vec3 norm = normalize(normal);
    vec3 color = get_sun_light(norm);
    FragColor = vec4(color, 1.0);
} 