#pragma once

// forward declare
class ShaderProgram;

class SurfaceMaterial
{
public:
    virtual void Update(ShaderProgram* shader_program) = 0;
};
