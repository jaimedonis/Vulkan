#include "shader_program.h"
#include "../diagnostics/logger.h"
#include "shader.h"
#include "object_manager.h"

#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>


constexpr GLsizei SHADER_LOG_BUFF_LEN = 1024 * 256;
static char g_sz_shader_buffer[SHADER_LOG_BUFF_LEN];


void ShaderProgram::AddShader(const std::string& shader_id)
{
    _shaders.push_back(shader_id);
}

void ShaderProgram::RemoveShader(const std::string& shader_id)
{
    _shaders.erase(
        std::remove_if(_shaders.begin(),
                       _shaders.end(),
                       [&](const std::string& item)
                       {
                           bool is_found = item == shader_id;
                           if(is_found && _gl_program_object != 0) {
                                auto shader = ObjectManager::Get<Shader>(shader_id);
                                if(shader) {
                                    GL_INVOKE(glDetachShader(_gl_program_object, shader->_gl_shader_object));
                                }
                           }
                           return is_found;
                       })
        );
}

void ShaderProgram::RemoveAllShaders()
{
    while(!_shaders.empty()) {
        RemoveShader(_shaders.front());
    }
}

bool ShaderProgram::Link()
{
    if( _shaders.empty()) {
        return false;
    }

    bool is_success;

    _gl_program_object = GL_INVOKE(glCreateProgram());
    is_success = (glGetError() == GL_NO_ERROR);

    for(const auto& shader_id : _shaders) {
        auto shader = ObjectManager::Get<Shader>(shader_id);
        assert(shader != nullptr);
        if(!shader->IsCompiled()) {
            is_success &= shader->Compile();
        }
        if(is_success) {
            GL_INVOKE(glAttachShader(_gl_program_object, shader->_gl_shader_object));
            is_success &= (glGetError() == GL_NO_ERROR);
        }
    }

    if( is_success ) {

        GL_INVOKE(glLinkProgram(_gl_program_object));
        is_success &= (glGetError() == GL_NO_ERROR);

        GLint param_value = 0;
        GL_INVOKE(glGetProgramiv(_gl_program_object, GL_LINK_STATUS, &param_value));
        is_success &= (param_value == GL_TRUE);
        if( is_success ) {
            std::cout << "The program object has been linked successfully." << std::endl;
        } else {
            std::cerr << "And ERROR has occurred linking the program object." << std::endl;
            GL_INVOKE(glGetProgramInfoLog(_gl_program_object, SHADER_LOG_BUFF_LEN,
                                          nullptr, g_sz_shader_buffer));
            std::cout << g_sz_shader_buffer << std::endl;
        }
    }

    return is_success;
}

void ShaderProgram::Destroy()
{
    // detach shaders without deleting them
    RemoveAllShaders();

    GL_INVOKE(glDeleteProgram(_gl_program_object));
    _gl_program_object = 0;
}

void ShaderProgram::BeginUsing()
{
    GL_INVOKE(glUseProgram(_gl_program_object));
}

void ShaderProgram::StopUsing()
{
    GL_INVOKE(glUseProgram(0));
}

void ShaderProgram::SetShaderUniform(const std::string& name, const glm::mat4& value) const
{
    GLint uniform_location = GL_INVOKE(glGetUniformLocation(_gl_program_object, name.c_str()));
    assert(uniform_location != -1);
    GL_INVOKE(glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value)));
}

void ShaderProgram::SetShaderUniform(const std::string& name, int value) const
{
    GLint uniform_location = GL_INVOKE(glGetUniformLocation(_gl_program_object, name.c_str()));
    assert(uniform_location != -1);
    GL_INVOKE(glUniform1i(uniform_location, value));
}

void ShaderProgram::SetShaderUniform(const std::string& name, float value) const
{
    GLint uniform_location = GL_INVOKE(glGetUniformLocation(_gl_program_object, name.c_str()));
    assert(uniform_location != -1);
    GL_INVOKE(glUniform1f(uniform_location, value));
}

void ShaderProgram::SetShaderUniform(const std::string& name, const glm::vec3& value) const
{
    GLint uniform_location = GL_INVOKE(glGetUniformLocation(_gl_program_object, name.c_str()));
    assert(uniform_location != -1);
    GL_INVOKE(glUniform3fv(uniform_location, 1, glm::value_ptr(value)));
}

void ShaderProgram::SetShaderUniform(const std::string& name, const glm::vec4& value) const
{
    GLint uniform_location = GL_INVOKE(glGetUniformLocation(_gl_program_object, name.c_str()));
    assert(uniform_location != -1);
    GL_INVOKE(glUniform4fv(uniform_location, 1, glm::value_ptr(value)));
}

void ShaderProgram::SetShaderUniform(const GLint uniform_location, const glm::mat4& value) const
{
    GL_INVOKE(glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value)));
}

void ShaderProgram::SetShaderUniform(const GLint uniform_location, int value) const
{
    GL_INVOKE(glUniform1i(uniform_location, value));
}

void ShaderProgram::SetShaderUniform(const GLint uniform_location, float value) const
{
    GL_INVOKE(glUniform1f(uniform_location, value));
}

void ShaderProgram::SetShaderUniform(const GLint uniform_location, const glm::vec3& value) const
{
    GL_INVOKE(glUniform3fv(uniform_location, 1, glm::value_ptr(value)));
}

void ShaderProgram::SetShaderUniform(const GLint uniform_location, const glm::vec4& value) const
{
    GL_INVOKE(glUniform4fv(uniform_location, 1, glm::value_ptr(value)));
}

void ShaderProgram::SetShaderBufferData(GLint buffer_object,
                                        int block_location,
                                        int binding_offset,
                                        void* buffer_data,
                                        size_t buffer_size)
{
    GL_INVOKE(glBindBuffer(GL_UNIFORM_BUFFER, buffer_object));
    GL_INVOKE(glBufferData(GL_UNIFORM_BUFFER,
                           buffer_size,
                           buffer_data,
                           GL_DYNAMIC_COPY));
    //GL_INVOKE(glUniformBlockBinding(_gl_program_object, block_location, binding_offset));
    GL_INVOKE(glBindBufferBase(GL_UNIFORM_BUFFER, block_location, buffer_object));
    GL_INVOKE(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
