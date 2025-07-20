#include "phong_surface_material.h"

#include "../graphics/shader_program.h"

PhongSurfaceMaterial::PhongSurfaceMaterial() :
    _diffuse_color(glm::vec3(0.0,1.0,0.0)),
    _specular_color(glm::vec3(0.05,0.05,0.05)),
    _s_sigma(0.45),
    _specular_power(pow(8192.0, 1.0-_s_sigma)),
    _surface_albedo(2.0)
{
}

void PhongSurfaceMaterial::Update(ShaderProgram* shader_program)
{
    int material_offset = 4; // TODO: remove
    //_shader_program->SetShaderBufferData(_surface_material_buffer_object,
    //                                     material_offset,
    //                                     0,
    //                                     &_surface_material,
    //                                     sizeof(SurfaceMaterial));
    shader_program->SetShaderUniform(material_offset, _diffuse_color);
    shader_program->SetShaderUniform(material_offset+1, _specular_color);
    shader_program->SetShaderUniform(material_offset+2, _specular_power);
    shader_program->SetShaderUniform(material_offset+3, _surface_albedo);
}
