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
    vec3 ambient_color;
    vec3 specular_color;
    float specular_power;
    float surface_albedo;
};

struct LightProperties {
    vec3 light_position;
    vec3 light_color;
};

layout(location = 4) uniform SurfaceMaterial material;
layout(location = 9) uniform LightProperties lighting;

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec3 vertex_normal;

layout (location = 0) out vec4 vertex_color;

invariant gl_Position;


vec3 ComputeDiffuseReflection(vec3 normal, vec3 light_direction)
{
    vec3 direct_color = lighting.light_color * dot(normal, light_direction);
    return (material.ambient_color + direct_color) * material.diffuse_color;
}

vec3 ComputeSpecularReflection(vec3 normal, vec3 half_vector, float nl)
{
    float highlight = pow(clamp(dot(normal, half_vector),0.0, 1.0), material.specular_power) * float(nl > 0.0);
    return lighting.light_color * material.specular_color * highlight;
}

vec3 Shade(vec3 world_position, vec3 normal)
{
    vec3 light_direction = normalize(lighting.light_position - world_position);
    vec3 diffuse = ComputeDiffuseReflection(normal, light_direction);

    vec3 eye_direction = normalize(camera_position - world_position);
    vec3 half_vector = normalize(light_direction + eye_direction);
    float nl = clamp(dot(normal, light_direction), 0.0, 1.0);
    vec3 specular = ComputeSpecularReflection(normal, half_vector, nl);

    float nh = clamp(dot(normal, half_vector), 0.0, 1.0);
    vec3 direct_color = (((material.surface_albedo / PI) * (diffuse * nl)) + (specular * pow(nh, material.specular_power))) * lighting.light_color;

    return direct_color;
}

void main()
{
    gl_Position = projection * view * model * vertex_position;
    vec4 world_position =  model * vertex_position;

    //vec3 normal = transpose(inverse(mat3(model))) * vertex_normal;
    vec3 normal = mat3(view * model) * vertex_normal; // same results as above

    vec3 color = Shade(world_position.xyz/world_position.w, normal);

    vertex_color = vec4(color, 1.0);
}
