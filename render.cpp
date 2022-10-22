#include "render.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "unit.h"

Render::Render(std::size_t width, std::size_t height) :
    text_program_(CreateProgram("text")),
    simple_program_(CreateProgram("simple")),
    texture_program_(CreateProgram("texture"))
{
    glm::mat4 projection = glm::ortho<float>(0.0f, width, height, 0.0f);

    glUseProgram(text_program_);
    glUniformMatrix4fv(glGetUniformLocation(text_program_, "u_projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(glGetUniformLocation(text_program_, "u_texture"), 0);

    InitText();

    glUseProgram(simple_program_);
    glUniformMatrix4fv(glGetUniformLocation(simple_program_, "u_projection"), 1, GL_FALSE, glm::value_ptr(projection));

    InitSimple();

    glUseProgram(texture_program_);
    glUniformMatrix4fv(glGetUniformLocation(texture_program_, "u_projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(glGetUniformLocation(texture_program_, "u_texture"), 0);

    InitTexture();

    glUseProgram(0);
}

Render::~Render()
{
    glDeleteVertexArrays(1, &text_vao);
    glDeleteBuffers(1, &text_vbo);

    glDeleteVertexArrays(1, &simple_vao);
    glDeleteBuffers(1, &simple_vbo);

    glDeleteVertexArrays(1, &texture_vao);
    glDeleteBuffers(1, &texture_vbo);
}

void Render::DrawTexture(GLuint texture_id, std::size_t x, std::size_t y, std::size_t w, std::size_t h, float a, float t)
{
    auto coords = GetVertices(x, y, w, h, a);

    glUseProgram(texture_program_);
        glActiveTexture(GL_TEXTURE0);
        glUniform1f(glGetUniformLocation(texture_program_, "u_transparence"), t);
        glBindVertexArray(texture_vao);
            glBindTexture(GL_TEXTURE_2D, texture_id);
                glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, coords.size()*sizeof(GLfloat), coords.data());
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    glUseProgram(0);
}

void Render::DrawText(const std::string &text, std::size_t x, std::size_t y, const Color &color, float scale)
{
    glUseProgram(text_program_);
        glActiveTexture(GL_TEXTURE0);
        glUniform3f(glGetUniformLocation(text_program_, "u_color"), color.x, color.y, color.z);
        glBindVertexArray(text_vao);
            for (auto ch: text)
            {
                Character character = characters_[ch];

                GLfloat xpos = x + character.bearing_.x * scale;
                GLfloat ypos = y - (character.size_.y - character.bearing_.y) * scale;

                GLfloat w = character.size_.x * scale;
                GLfloat h = character.size_.y * scale;

                GLfloat vertices[6][4] = {
                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos,     ypos,       0.0, 1.0 },
                    { xpos + w, ypos,       1.0, 1.0 },

                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos + w, ypos,       1.0, 1.0 },
                    { xpos + w, ypos + h,   1.0, 0.0 }
                };
                glBindTexture(GL_TEXTURE_2D, character.texture_id_);
                    glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    x += (character.advance_ >> 6) * scale;
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        glBindVertexArray(0);
        glUseProgram(0);
}

void Render::DrawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, const Color& color)
{
    GLfloat coords[] = {
        x - 0.5f*w, y - 0.5f*h,
        x - 0.5f*w, y + 0.5f*h,
        x + 0.5f*w, y + 0.5f*h,

        x - 0.5f*w, y - 0.5f*h,
        x + 0.5f*w, y + 0.5f*h,
        x + 0.5f*w, y - 0.5f*h
    };

    glUseProgram(simple_program_);
        glUniform4f(glGetUniformLocation(simple_program_, "u_color"), color.r, color.g, color.b, color.a);
        glBindVertexArray(simple_vao);
            glBindBuffer(GL_ARRAY_BUFFER, simple_vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    glUseProgram(0);
}

void Render::DrawLine(std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1, const Color &color)
{
    glm::vec2 v = glm::vec2{x0, y0} - glm::vec2{x1, y1};
    glm::vec2 n;

    if(v.x != 0)
        n = glm::normalize(glm::vec2{ -v.y/v.x, 1.0f });
    else
        n = glm::normalize(glm::vec2{ 1.0f, -v.x/v.y });

    GLfloat coords[] = {
        x0 + n.x, y0 + n.y,
        x0 - n.x, y0 - n.y,
        x1 + n.x, y1 + n.y,
        x0 + n.x, y0 + n.y,
        x1 + n.x, y1 + n.y,
        x1 - n.x, y1 - n.y
    };

    glUseProgram(simple_program_);
        glUniform4f(glGetUniformLocation(simple_program_, "u_color"), color.r, color.g, color.b, color.a);
        glBindVertexArray(simple_vao);
            glBindBuffer(GL_ARRAY_BUFFER, simple_vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coords), coords);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
}

void Render::DrawUnit(const ::Unit &unit, GLfloat scale)
{
    const auto& unit_ = units_.at(unit.type_);

    DrawTexture(unit_.texture_id_, unit.position_.x, unit.position_.y, unit_.width_*scale, unit_.height_*scale, unit.rotation_);
}

void Render::AddUnitTexture(GLuint texture_id, UnitType type, GLfloat w, GLfloat h)
{
    units_.insert({type, {texture_id, w, h}});
}

GLuint Render::LoadTexture(const std::string &path, std::size_t w, std::size_t h)
{
    std::ifstream in{ path, std::ios_base::binary | std::ios_base::in };

    if(!in.is_open())
        throw std::runtime_error{ "File " + path + " not found" };

    std::vector< char > data{ std::istreambuf_iterator< char >{ in },
                              std::istreambuf_iterator< char >{} };

    return CreateTexture(GL_RGBA, w, h, data.data());
}

GLuint Render::CreateTexture(GLenum type, std::size_t w, std::size_t h, const GLchar* data)
{
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void Render::InitTexture()
{
    glGenVertexArrays(1, &texture_vao);
    glGenBuffers(1, &texture_vbo);

    glBindVertexArray(texture_vao);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Render::InitSimple()
{
    glGenVertexArrays(1, &simple_vao);
    glGenBuffers(1, &simple_vbo);

    glBindVertexArray(simple_vao);
        glBindBuffer(GL_ARRAY_BUFFER, simple_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Render::InitText()
{
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
        throw std::runtime_error{"FT initiation failed"};

    FT_Face face;
    if (FT_New_Face(ft, "fonts/Kinnari.ttf", 0, &face))
        throw std::runtime_error{"Font loading failed"};

    FT_Set_Pixel_Sizes(face, 0, 24);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            throw std::runtime_error{"Font loading failed"};

        GLuint texture = CreateTexture(GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, (GLchar*)face->glyph->bitmap.buffer);

        characters_.emplace(c, Character{
                                texture,
                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                static_cast<GLuint>(face->glyph->advance.x)
                            });
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &text_vao);
    glGenBuffers(1, &text_vbo);

    glBindVertexArray(text_vao);
        glBindBuffer(GL_ARRAY_BUFFER, text_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint Render::CreateProgram(const std::string &name)
{
    auto program = glCreateProgram();

    auto vertex_shader = LoadShader("shaders/" + name + ".vert", GL_VERTEX_SHADER);
    auto fragment_shader = LoadShader("shaders/" + name + ".frag", GL_FRAGMENT_SHADER);

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

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

GLuint Render::LoadShader(const std::string &path, GLenum type)
{
    std::ifstream in{ path };

    if(!in.is_open())
        throw std::runtime_error{ "File " + path + " not found" };

    std::string code{ std::istreambuf_iterator< char >{ in },
                      std::istreambuf_iterator< char >{} };

    const GLchar* code_p = code.c_str();

    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &code_p, NULL);
    glCompileShader(shader);

    GLint ok{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if(!ok)
    {
        GLchar error[512];
        glGetShaderInfoLog(shader, 512, NULL, error);
        throw std::runtime_error{ error };
    };

    return shader;
}

std::array<GLfloat, 24> Render::GetVertices(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat a)
{
    glm::mat4 mat{ 1.0f };
    mat = glm::rotate(mat, -glm::radians(a - 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    auto lt = mat*glm::vec4(-0.5*w, +0.5f*h, 0.0f, 0.0f) + glm::vec4(x, y, 0.0f, 0.0f);
    auto lb = mat*glm::vec4(-0.5*w, -0.5f*h, 0.0f, 0.0f) + glm::vec4(x, y, 0.0f, 0.0f);
    auto rb = mat*glm::vec4(+0.5*w, -0.5f*h, 0.0f, 0.0f) + glm::vec4(x, y, 0.0f, 0.0f);
    auto rt = mat*glm::vec4(+0.5*w, +0.5f*h, 0.0f, 0.0f) + glm::vec4(x, y, 0.0f, 0.0f);

    return {
        lt.x, lt.y, 0.0f, 1.0f,
        lb.x, lb.y, 0.0f, 0.0f,
        rb.x, rb.y, 1.0f, 0.0f,

        lt.x, lt.y, 0.0f, 1.0f,
        rb.x, rb.y, 1.0f, 0.0f,
        rt.x, rt.y, 1.0f, 1.0f
    };
}
