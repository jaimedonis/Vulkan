#version 430 core

#extension GL_ARB_explicit_uniform_location : require

#define PI 3.1415927

layout(std430, column_major) uniform;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

layout(location = 3) uniform vec3 camera_position;

struct SurfaceMaterial {
    vec3 diffuse_color;
    vec3 specular_color;
    float specular_power;
    float surface_albedo;
};

struct LightProperties {
    vec3 light_position;
    vec3 light_power;
    vec3 light_color; // environment light color
};

layout(location = 4) uniform SurfaceMaterial material;
layout(location = 8) uniform LightProperties lighting;

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec3 vertex_normal;

layout (location = 0) out vec4 frag_color;

invariant gl_Position;


bool Visible(vec3 point,
             vec3 w_i,
             float distance_to_light)
{
    return true; // uses uniform sampler2DShadow shadow_map (depth map from light position)
}

vec3 EvaluateBSDF(vec3 w_i, vec3 w_o, vec3 n)
{
    // w_i: incomming light ray attenuation...?
    // w_o: vector from surface point to eye
    // diffuse_color: lambertian diffuse color
    // specular_color: glossiness
    // s: specular power
    // n: surface normal
    vec3 w_h = normalize(w_i + w_o);
    vec3 result = (material.surface_albedo / PI) *
            (material.diffuse_color + material.specular_color *
             ((material.specular_power + 8.0) * pow(max(0.0, dot(w_h, n)), material.specular_power) / 8.0));

    return result;
}

vec3 Shade(vec3 point, vec3 n)
{
    vec3 radiance = vec3(0.0);

    vec3 offset = lighting.light_position - point;
    float distance_to_light = length(offset);
    vec3 w_i = offset / distance_to_light;
    vec3 offset_eye = camera_position - point;
    float distance_to_eye = length(offset_eye);
    //vec3 w_o = normalize(offset_eye); // original equation
    vec3 w_o = normalize(offset_eye / distance_to_eye);

    vec3 environment_light = lighting.light_color * dot(w_i, w_o);

    if( Visible(point, w_i, distance_to_light) ) {
        vec3 L_i = (lighting.light_power / (4 * PI * distance_to_light * distance_to_light));
        radiance += L_i * EvaluateBSDF(w_i, w_o, n) * max(0.0, dot(w_i, n));
        radiance += environment_light;
    }

    return radiance;
}

void main()
{
    gl_Position = projection * view * model * vertex_position;
    vec4 world_position =  model * vertex_position;

    //vec3 normal = transpose(inverse(mat3(model))) * vertex_normal;
    vec3 normal = mat3(view * model) * vertex_normal; // same results as above

    vec3 color = Shade(world_position.xyz, normal);
    float alpha = 1.0;

    frag_color = vec4(color, alpha);
}
