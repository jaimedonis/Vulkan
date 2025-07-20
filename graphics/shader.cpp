#include "shader.h"
#include "../diagnostics/logger.h"

#include <fstream>
#include <stack>
#include <iostream>

constexpr GLsizei SHADER_LOG_BUFF_LEN = 1024 * 256;
static char g_sz_shader_buffer[SHADER_LOG_BUFF_LEN];


static GLenum ToGLShaderType(const ShaderType shader_type)
{
    GLenum gl_shader = -1;

    switch(shader_type) {
    case ShaderType::VERTEX:
        gl_shader = GL_VERTEX_SHADER;
        break;
    case ShaderType::FRAGMENT:
        gl_shader = GL_FRAGMENT_SHADER;
        break;
    case ShaderType::GEOMETRY:
        gl_shader = GL_GEOMETRY_SHADER;
        break;
    case ShaderType::COMPUTE:
        gl_shader = GL_COMPUTE_SHADER;
        break;
    default:
        gl_shader = -1;
        break;
    }

    return gl_shader;
}

Shader::Shader(const ShaderType shader_type, const std::string& shader_path)
    : _shader_type(shader_type),
    _shader_path(shader_path),
    _gl_shader_object(0),
    _is_compiled(false)
{
}

bool Shader::Compile()
{
    std::string all_shader_source;
    bool is_success = GetShaderSource(_shader_path, all_shader_source);

    _gl_shader_object = GL_INVOKE(glCreateShader(ToGLShaderType(_shader_type)));

    const char* sz_shader_source = all_shader_source.c_str();
    GL_INVOKE(glShaderSource(_gl_shader_object, 1, &sz_shader_source, NULL));
    is_success &= (glGetError() == GL_NO_ERROR);

    GL_INVOKE(glCompileShader(_gl_shader_object));
    GLint param_value = 0;
    GL_INVOKE(glGetShaderiv(_gl_shader_object, GL_COMPILE_STATUS, &param_value));
    is_success &= (param_value == GL_TRUE);
    if( is_success ) {
        std::cout << "The shader object has been compiled successfully: " << _shader_path << std::endl;
    } else {
        std::cerr << "And ERROR has occurred compiling the shader object: " << _shader_path << std::endl;
        GL_INVOKE(glGetShaderInfoLog(_gl_shader_object, SHADER_LOG_BUFF_LEN,
                                     nullptr, g_sz_shader_buffer));
        std::cout << g_sz_shader_buffer << std::endl;
        exit(1);
    }

    _is_compiled = is_success;

    return _is_compiled;
}

void Shader::Destroy()
{
    GL_INVOKE(glDeleteShader(_gl_shader_object));
    _gl_shader_object = 0;
    _is_compiled = false;
}

bool Shader::GetShaderSource(const std::string& shader_path, std::string& all_shader_source)
{
    std::queue<std::string> shader_includes_queue;
    std::stack<std::string> all_shader_source_stack;

    shader_includes_queue.push(shader_path);
    int counter = 0;

    while(!shader_includes_queue.empty()) {
        std::string include_file_name = shader_includes_queue.front();
        shader_includes_queue.pop();

        auto f_stream = std::ifstream(include_file_name);
        if(!f_stream.is_open()) {
            std::cout << "ERROR opening shader source file: " << shader_path << std::endl;
            return false;
        }
        std::string shader_text;
        std::string line;
        while(std::getline(f_stream, line)) {
            line += "\n";
            if(!GetIncludes(line, shader_includes_queue)) {
                shader_text += line;
            }
        }
        f_stream.close();
        counter++;

        if( !shader_text.empty() ) {
            all_shader_source_stack.push(shader_text);            
        }
    }

    while(!all_shader_source_stack.empty()) {
        all_shader_source += all_shader_source_stack.top();
        all_shader_source_stack.pop();
    }

    std::cout << "=============================================" << std::endl;
    std::cout << shader_path << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << all_shader_source << std::endl;
    std::cout << "=============================================" << std::endl;

    return counter > 0;
}

bool Shader::GetIncludes(const std::string& shader_source, std::queue<std::string>& includes)
{
    const std::string include_string = "#include";
    const std::string eol_string = "\n"; // TODO: support Windows OS "\r\n"

    long found_pos = -1;
    int counter = 0;
    while((found_pos = shader_source.find(include_string, found_pos+1)) >= 0)
    {
        long token_content_start = found_pos + include_string.length() + 1;
        long token_content_end = shader_source.find(eol_string, token_content_start);
        if( token_content_end >= 0 ) {
            std::string token_content = shader_source.substr(token_content_start, token_content_end - token_content_start);
            includes.push(token_content);
            found_pos = token_content_end;
            counter++;
        } else {
            break;
        }
    }

    return counter > 0;
}
