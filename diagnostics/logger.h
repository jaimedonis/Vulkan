#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

#define GL_INVOKE(x) x; Logging::LogOpenGLError(#x, __FILE__, __LINE__);

namespace Logging
{
void LogOpenGLError(const std::string& gl_command, const std::string& source_file, int source_line);
}
