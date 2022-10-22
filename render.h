#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "primitives.h"
#include "unit.h"

class Render
{
    struct Character {
        GLuint texture_id_;
        glm::ivec2 size_;
        glm::ivec2 bearing_;
        GLuint advance_;
    };

    struct Unit
    {
        GLuint texture_id_;
        GLfloat width_;
        GLfloat height_;
    };
public:
    Render(std::size_t width, std::size_t height);

    ~Render();

    void DrawTexture(GLuint texture_id, std::size_t x, std::size_t y, std::size_t w, std::size_t h, float a = 90.0f, float t = 1.0f);

    void DrawText(const std::string& text, std::size_t x, std::size_t y, const Color& color, float scale = 1.0f);

    void DrawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, const Color& color);

    void DrawLine(std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1, const Color& color);

    void DrawUnit(const ::Unit& unit, GLfloat scale = 1.0f);

    void AddUnitTexture(GLuint texture_id, UnitType type, GLfloat w, GLfloat h);

    static GLuint LoadTexture(const std::string& path, std::size_t w, std::size_t h);

    static GLuint CreateTexture(GLenum type, std::size_t w, std::size_t h, const GLchar* data);
private:
    void InitTexture();
    void InitSimple();
    void InitText();

    GLuint CreateProgram(const std::string& name);

    GLuint LoadShader(const std::string& path, GLenum type);

    std::array<GLfloat, 24> GetVertices(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat a);

    GLuint texture_program_;
    GLuint simple_program_;
    GLuint text_program_;

    GLuint texture_vbo, texture_vao;
    GLuint simple_vbo, simple_vao;
    GLuint text_vbo, text_vao;

    std::unordered_map<char, Character> characters_;

    std::unordered_map<UnitType, Unit> units_;
};
