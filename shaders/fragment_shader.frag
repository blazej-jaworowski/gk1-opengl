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

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 position;
    vec3 direction;
    float cutoff;
};

#define DIR_LIGHT_COUNT 2
#define SPOT_LIGHT_COUNT 2

uniform Material material;
uniform DirLight dir_lights[DIR_LIGHT_COUNT];
uniform SpotLight spot_lights[SPOT_LIGHT_COUNT];

uniform vec3 camera_pos;
uniform bool blinn_reflection;
uniform bool fog_enabled;
uniform float fog_dist;
uniform vec3 fog_color;

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

vec3 get_spot_light(vec3 norm) {
    vec3 sum = vec3(0);
    for(int i = 0; i < SPOT_LIGHT_COUNT; i++) {
        vec3 dir = normalize(frag_pos - spot_lights[i].position);
        float theta = dot(spot_lights[i].direction,
                          dir);

        if(theta < spot_lights[i].cutoff) {
            continue;
        }

        float distance    = length(spot_lights[i].position - frag_pos);
        float attenuation = 1.0 / (spot_lights[i].constant +
                                   spot_lights[i].linear * distance + 
    		                       spot_lights[i].quadratic * (distance * distance));

        vec3 ambient = spot_lights[i].ambient * material.ambient;

        vec3 diffuse = max(dot(-dir, norm), 0.0)
                     * spot_lights[i].diffuse * material.diffuse;

        vec3 view_dir = normalize(camera_pos - frag_pos);
        vec3 specular;
        if(blinn_reflection) {
            vec3 half_dir = normalize(dir + view_dir);
            float spec_angle = max(dot(half_dir, norm), 0.0);
            specular = pow(spec_angle, material.shininess * 4)
                       * spot_lights[i].specular * material.specular;
        } else {
            vec3 reflect_dir = reflect(-dir, norm);
            float spec_angle = max(dot(view_dir, reflect_dir), 0.0);
            specular = pow(spec_angle, material.shininess)
                        * spot_lights[i].specular * material.specular;
        }
        sum += (ambient + diffuse + specular) * attenuation;
    }
    return sum;
}

vec3 apply_fog(vec3 color, float ls, float le, vec3 fog_color) {
    float dist = gl_FragCoord.z / gl_FragCoord.w;

    float k = 1.0 - (le - dist) / (le - ls);
    k = min(max(k, 0), 1);

    return mix(color, fog_color, k);
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 color = get_dir_light(norm) + get_spot_light(norm);
    if(fog_enabled) {
        color = apply_fog(color, 2, fog_dist, fog_color);
    }
    FragColor = vec4(color, 1.0);
} 