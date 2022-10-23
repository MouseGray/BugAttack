#pragma once

#include <string>

#include "openglobject.h"

namespace opengl
{

class Shader : public OpenGLObject
{
public:
    enum class Type
    {
        Vertex,
        Fragment
    };

    Shader(Type type, const std::string& path);

    ~Shader();
private:
    std::string LoadCode(const std::string& path) const;

    GLuint CreateShader(Type type, const GLchar* code) const;

    GLenum CastToGLType(Type type) const noexcept;
};

}
