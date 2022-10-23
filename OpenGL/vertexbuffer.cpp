#include "vertexbuffer.h"

namespace opengl
{

VertexBuffer::VertexBuffer(std::size_t size)
{
    handle_ = CreateVertexBuffer(size);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &handle_);
}

VertexBuffer::Using VertexBuffer::Use() noexcept
{
    return Using{handle_};
}

void VertexBuffer::SetData(GLfloat *data, std::size_t size)
{
    auto _ = Use();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(GLfloat), data);
}

GLuint opengl::VertexBuffer::CreateVertexBuffer(std::size_t size)
{
    GLuint vertex_buffer;

    glGenBuffers(1, &vertex_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertex_buffer;
}

}
