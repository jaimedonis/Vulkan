#version 410 core

#define PI 3.1415927

uniform vec3 light_position = vec3(0, 5, 4);
uniform vec3 light_power = vec3(5, 5, 5);

uniform vec3 k_L = vec3(1, 0, 0);
uniform vec3 k_G = vec3(1, 1, 1);
uniform float s = 50.0;


bool Visible(vec3 point,
             vec3 w_i,
             float distance_to_light)
{
    return true; // uses uniform sampler2DShadow shadow_map (depth map from light position)
}

vec3 EvaluateBSDF(vec3 w_i, vec3 w_o, vec3 n)
{
    // evaluateFiniteScatteringDensity.
    // w_i: incomming light ray attenuation...?
    // w_o: -normalize(point)
    // k_L: lambertian diffuse color
    // k_G: glossiness
    // s: specular power.  e.g. ~ 100.0
    // n: surface normal

    vec3 w_h = normalize(wi + w_o);
    vec3 result = (k_L + k_G * ((s + 8.0) * pow(max(0.0, dot(w_h, n)), s) / 8.0)) / PI;
    return result;
}

vec3 Shade(vec3 point, vec3 n)
{
    vec3 radiance = vec3(0.0)

    vec3 offset = light_position - point;
    float distance_to_light = length(offset);
    vec3 w_i = offset / distance_to_light;
    vec3 w_o = -normalize(point);

    if( Visible(point, w_i, distance_to_light) ) {
        vec3 L_i = light_power / (4 * PI * distance_to_light * distance_to_light);
        radiance += L_i * EvaluateBSDF(w_i, w_o, n) * max(0.0, dot(w_i, n));
    }

    return radiance;
}
