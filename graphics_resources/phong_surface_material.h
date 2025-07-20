#pragma once

#include "surface_material.h"

#include <glm/glm.hpp>

class PhongSurfaceMaterial : public SurfaceMaterial
{
public:
    PhongSurfaceMaterial();

public:
    // public properties
    // ...

    // overrides

    void Update(ShaderProgram* shader_program) override;

private:
    glm::vec3 _diffuse_color;
    glm::vec3 _specular_color;
    float _s_sigma;
    glm::vec3 _specular_power ;
    glm::vec3 _surface_albedo;
};

