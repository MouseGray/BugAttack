#pragma once

#include "opengl.h"
#include "shader.h"

namespace opengl
{

class Program : public OpenGLObject
{
public:
    Program(const Shader& vertex_shader, const Shader& fragment_shader);

    ~Program();

    void SetParam(std::string_view name, GLint v);

    void SetParam(std::string_view name, GLfloat v);

    void SetParam(std::string_view name, GLfloat v1, GLfloat v2, GLfloat v3);

    void SetParam(std::string_view name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);

    void SetParam(std::string_view name, const GLfloat* v);

private:
    GLuint CreateProgram(const Shader& vertex_shader, const Shader& fragment_shader);
};

struct UseProgram : Using
{
    UseProgram(const Program& program) { glUseProgram(program); }

    ~UseProgram() { glUseProgram(0); }
};

}

