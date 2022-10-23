#include "openglobject.h"

namespace opengl
{

OpenGLObject::operator GLuint() const & noexcept
{
    return handle_;
}

OpenGLObject::~OpenGLObject()
{

}

}
