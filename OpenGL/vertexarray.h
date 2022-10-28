#pragma once

#include "opengl.h"
#include "vertexbuffer.h"

namespace opengl
{

class VertexArray : public OpenGLObject
{
public:
    VertexArray(std::size_t size, std::size_t offset);

    ~VertexArray();

    template<std::size_t Size>
    void SetData(std::array<GLfloat, Size> data)
    {
        SetDataImpl(data.data(), Size);
    }
private:
    GLuint CreateVertexArray(std::size_t offset);

    void SetDataImpl(GLfloat* data, std::size_t size);

    VertexBuffer buffer_;
};

struct UseVertexArray : Using
{
    UseVertexArray(const VertexArray& vertex_array) { glBindVertexArray(vertex_array); }

    ~UseVertexArray() { glBindVertexArray(0); }
};

}
