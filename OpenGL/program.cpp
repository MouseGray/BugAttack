#include "program.h"

#include <stdexcept>

namespace opengl
{

Program::Program(const Shader &vertex_shader, const Shader &fragment_shader)
{
    handle_ = CreateProgram(vertex_shader, fragment_shader);
}

Program::~Program()
{
    glDeleteProgram(handle_);
}

void Program::SetParam(std::string_view name, GLint v)
{
    USE(Program, *this);

    auto location = glGetUniformLocation(handle_, name.data());

    if(location == -1)
        throw std::invalid_argument{"Param '" + std::string{name} + "' not found"};

    glUniform1i(location, v);
}

void Program::SetParam(std::string_view name, GLfloat v)
{
    USE(Program, *this);

    auto location = glGetUniformLocation(handle_, name.data());

    if(location == -1)
        throw std::invalid_argument{"Param '" + std::string{name} + "' not found"};

    glUniform1f(location, v);
}

void Program::SetParam(std::string_view name, GLfloat v1, GLfloat v2, GLfloat v3)
{
    USE(Program, *this);

    auto location = glGetUniformLocation(handle_, name.data());

    if(location == -1)
        throw std::invalid_argument{"Param '" + std::string{name} + "' not found"};

    glUniform3f(location, v1, v2, v3);
}

void Program::SetParam(std::string_view name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4)
{
    USE(Program, *this);

    auto location = glGetUniformLocation(handle_, name.data());

    if(location == -1)
        throw std::invalid_argument{"Param '" + std::string{name} + "' not found"};

    glUniform4f(location, v1, v2, v3, v4);
}

void Program::SetParam(std::string_view name, const GLfloat* v)
{
    USE(Program, *this);

    auto location = glGetUniformLocation(handle_, name.data());

    if(location == -1)
        throw std::invalid_argument{"Param '" + std::string{name} + "' not found"};

    glUniformMatrix4fv(location, 1, GL_FALSE, v);
}

GLuint Program::CreateProgram(const Shader &vertex_shader, const Shader &fragment_shader)
{
    auto program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint ok{};

    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if(!ok)
    {
        GLchar error[512];
        glGetProgramInfoLog(program, 512, NULL, error);
        throw std::runtime_error{ error };
    };

    return program;
}

}
