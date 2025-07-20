#include "logger.h"

#include <iostream>
#include <cassert>

namespace Logging
{

static std::string GLErrorToString(GLenum err)
{
    switch (err) {
    case GL_NO_ERROR:
        return "No Error: No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
    case GL_INVALID_ENUM:
        return "Invalid enum: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:
        return "Invalid value: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
        return "Invalid operation: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "Invalid framebuffer operation: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
        return "Out of memory: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    case GL_STACK_UNDERFLOW:
        return "Stack Underflow: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "Stack overflow: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        break;
    }
    return "Unspecified error";
}

void LogOpenGLError(const std::string& gl_command, const std::string& source_file, int source_line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << " -- " << GLErrorToString(err) << std::endl;
        std::cerr << "Relevant Command: " << gl_command << std::endl;
        std::cerr << "Location: " << source_file << " at Line: " << source_line << std::endl;
    }
    assert(err == GL_NO_ERROR);
}

}
