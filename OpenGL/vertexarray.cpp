#include "vertexarray.h"

namespace opengl
{

VertexArray::VertexArray(std::size_t size, std::size_t offset) :
    buffer_{size}
{
    handle_ = CreateVertexArray(offset);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &handle_);
}

GLuint VertexArray::CreateVertexArray(std::size_t offset)
{
    GLuint vertex_array;

    glGenVertexArrays(1, &vertex_array);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, offset, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertex_array;
}

void VertexArray::SetDataImpl(GLfloat *data, std::size_t size)
{
    USE(VertexArray, *this);
    buffer_.SetData(data, size);
}

}
