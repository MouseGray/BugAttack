#pragma once

#include "vertexbuffer.h"

namespace opengl
{

class VertexArray : public OpenGLObject
{
    struct Using
    {
        Using(GLuint handle) { glBindVertexArray(handle); }
        ~Using() { glBindVertexArray(0); };
    };

public:
    VertexArray(std::size_t size, std::size_t offset);

    ~VertexArray();

    [[nodiscard]]
    Using Use() noexcept;

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

}
