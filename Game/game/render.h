#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include <unit/unit.h>
#include <OpenGL/program.h>
#include <OpenGL/vertexarray.h>

namespace bugattack{class Map;}
namespace bugattack::tower{class TowerBase;}

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

    void DrawTexture(GLuint texture_id, std::size_t x, std::size_t y, std::size_t w, std::size_t h, float a = 0.0f, float t = 1.0f);

    void DrawText(const std::string& text, std::size_t x, std::size_t y, const glm::vec4& color, float scale = 1.0f);

    void DrawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, const glm::vec4& color);

    void DrawLine(std::size_t x0, std::size_t y0, std::size_t x1, std::size_t y1, const glm::vec4& color);

    void DrawUnit(const bugattack::Unit& unit, GLfloat scale = 1.0f);

    void AddUnitTexture(GLuint texture_id, bugattack::UnitType type, GLfloat w, GLfloat h);

    static GLuint LoadTexture(const std::string& path, std::size_t w, std::size_t h);

    static GLuint CreateTexture(GLenum type, std::size_t w, std::size_t h, const GLchar* data);
private:
    void InitText();

    std::array<GLfloat, 24> GetVertices(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat a);

    opengl::Program texture_program_;
    opengl::Program simple_program_;
    opengl::Program text_program_;

    opengl::VertexArray texture_vao;
    opengl::VertexArray simple_vao;
    opengl::VertexArray text_vao;

    std::unordered_map<char, Character> characters_;

    std::unordered_map<bugattack::UnitType, Unit> units_;
};
