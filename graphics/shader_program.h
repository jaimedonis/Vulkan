#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

class ShaderProgram
{
public:
    ShaderProgram() = default;

public:
    // TBD: Rename to AttachShader, DetachShader(), DetachAllShaders()..?
    //      When we add a shader to the object store, add it only iff it compiles successfully.
    //      Rename obect store to shader store
    //      Shader Store adds shaders to shader groups
    //      Destroy shaders one by one, or by group.
    //      Level/World loader adds shaders to shader store and destroys them.
    void AddShader(const std::string& shader_id);
    void RemoveShader(const std::string& shader_id);
    void RemoveAllShaders();

    bool Link();
    void Destroy();

    void BeginUsing();
    void StopUsing();

    void SetShaderUniform(const std::string& name, const glm::mat4& value) const;
    void SetShaderUniform(const std::string& name, int value) const;
    void SetShaderUniform(const std::string& name, float value) const;
    void SetShaderUniform(const std::string& name, const glm::vec3& value) const;
    void SetShaderUniform(const std::string& name, const glm::vec4& value) const;

    void SetShaderUniform(const GLint uniform_location, const glm::mat4& value) const;
    void SetShaderUniform(const GLint uniform_location, int value) const;
    void SetShaderUniform(const GLint uniform_location, float value) const;
    void SetShaderUniform(const GLint uniform_location, const glm::vec3& value) const;
    void SetShaderUniform(const GLint uniform_location, const glm::vec4& value) const;

    void SetShaderBufferData(GLint buffer_object,
                             int block_location,
                             int binding_offset,
                             void* buffer_data,
                             size_t buffer_size);

private:

    GLint _gl_program_object = 0;
    std::vector<std::string> _shaders;

};

