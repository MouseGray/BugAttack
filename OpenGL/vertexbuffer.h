#pragma once

#include <array>

#include "opengl.h"
#include "openglobject.h"

namespace opengl
{

class VertexBuffer : public OpenGLObject
{
public:
    VertexBuffer(std::size_t size);

    ~VertexBuffer();

    void SetData(GLfloat* data, std::size_t size);
private:
    GLuint CreateVertexBuffer(std::size_t size);
};

struct UseVertexBuffer : Using
{
    UseVertexBuffer(const VertexBuffer& vertex_buffer) { glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); }

    ~UseVertexBuffer() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

}
