#pragma once

#include <GL/glew.h>

namespace opengl
{

class OpenGLObject
{
public:
    OpenGLObject() = default;

    operator GLuint() const & noexcept;
protected:
    virtual ~OpenGLObject() = 0;

    GLuint handle_;
};

}
