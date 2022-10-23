#include "shader.h"

#include <cassert>
#include <fstream>

namespace opengl
{

Shader::Shader(Type type, const std::string &path)
{
    auto code = LoadCode(path);
    handle_ = CreateShader(type, code.c_str());
}

Shader::~Shader()
{
    glDeleteShader(handle_);
}

std::string Shader::LoadCode(const std::string &path) const
{
    std::ifstream in{path};

    if(!in.is_open())
        throw std::runtime_error{ "File " + path + " not found" };

    return { std::istreambuf_iterator< char >{ in },
        std::istreambuf_iterator< char >{} };
}

GLuint Shader::CreateShader(Type type, const GLchar *code) const
{
    auto shader = glCreateShader(CastToGLType(type));
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    GLint ok{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if(!ok)
    {
        GLchar error[512];
        glGetShaderInfoLog(shader, 512, NULL, error);
        throw std::runtime_error{ error };
    };

    return shader;
}

GLenum Shader::CastToGLType(Type type) const noexcept
{
    switch (type)
    {
        case Type::Vertex: return GL_VERTEX_SHADER;
        case Type::Fragment: return GL_FRAGMENT_SHADER;
        default: assert(false && "Invalid shader type");
    }
    return 0;
}

}
