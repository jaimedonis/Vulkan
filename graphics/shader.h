#pragma once

#include "managed_object.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include <string>
#include <queue>

//! Forward declarations

class ShaderProgram;

//! Shader type enum
enum class ShaderType
{
    UNKNOWN,
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    COMPUTE
};

//! Generic shader class
class Shader : public ManagedObject
{
public:
    Shader(const ShaderType shader_type, const std::string& shader_path);
    virtual ~Shader() = default;

public:
    bool Compile();

    void Destroy();

    inline bool IsCompiled() const { return _is_compiled; }

private:
    bool GetShaderSource(const std::string& shader_path, std::string& all_shader_source);
    bool GetIncludes(const std::string& shader_source, std::queue<std::string>& includes);

private:

    ShaderType _shader_type = ShaderType::UNKNOWN;
    std::string _shader_path;

    GLuint _gl_shader_object;
    bool _is_compiled;

    friend class ShaderProgram;
};
