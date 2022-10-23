#pragma once

#include <array>

#include "openglobject.h"

namespace opengl
{

class VertexBuffer : public OpenGLObject
{
    struct Using
    {
        Using(GLuint handle) { glBindBuffer(GL_ARRAY_BUFFER, handle); }
        ~Using() { glBindBuffer(GL_ARRAY_BUFFER, 0); };
    };

public:
    VertexBuffer(std::size_t size);

    ~VertexBuffer();

    [[nodiscard]]
    Using Use() noexcept;

    void SetData(GLfloat* data, std::size_t size);
private:
    GLuint CreateVertexBuffer(std::size_t size);
};

}
