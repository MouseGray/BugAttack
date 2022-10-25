#include "render.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <OpenGL/shader.h>

#include <tower/gun.h>
#include <tower/laser.h>
#include <tower/rocket.h>

#include "unit.h"

Render::Render(std::size_t width, std::size_t height) :
    text_program_{
        {opengl::Shader::Type::Vertex, "shaders/text.vert"},
        {opengl::Shader::Type::Fragment, "shaders/text.frag"}
    },
    simple_program_{
        {opengl::Shader::Type::Vertex, "shaders/simple.vert"},
        {opengl::Shader::Type::Fragment, "shaders/simple.frag"}
    },
    texture_program_{
        {opengl::Shader::Type::Vertex, "shaders/texture.vert"},
        {opengl::Shader::Type::Fragment, "shaders/texture.frag"}
    },
    text_vao   {24, 4},
    simple_vao {12, 2},
    texture_vao{24, 4}
{
    glm::mat4 projection = glm::ortho<float>(0.0f, width, height, 0.0f);

    text_program_.SetParam("u_projection", glm::value_ptr(projection));
    text_program_.SetParam("u_texture", 0);

    InitText();

    simple_program_.SetParam("u_projection", glm::value_ptr(projection));

    texture_program_.SetParam("u_projection", glm::value_ptr(projection));
    texture_program_.SetParam("u_texture", 0);
}

void Render::DrawTexture(GLuint texture_id, std::size_t x, std::size_t y, std::size_t w, std::size_t h, float a, float t)
{
    auto coords = GetVertices(x, y, w, h, a);

    texture_program_.SetParam("u_transparence", t);
    texture_vao.SetData(coords);

    glUseProgram(texture_program_);
        glBindVertexArray(texture_vao);
            glBindTexture(GL_TEXTURE_2D, texture_id);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    glUseProgram(0);
}

void Render::DrawText(const std::string &text, std::size_t x, std::size_t y, const Color &color, float scale)
{
    text_program_.SetParam("u_color", color.r, color.g, color.b);

    glUseProgram(text_program_);

    for (auto ch: text)
    {
        Character character = characters_[ch];

        GLfloat xpos = x + character.bearing_.x * scale;
        GLfloat ypos = y - (character.size_.y - character.bearing_.y) * scale;

        GLfloat w = character.size_.x * scale;
        GLfloat h = character.size_.y * scale;

        std::array<GLfloat, 24> vertices = {
            xpos,     ypos + h,   0.0, 0.0,
            xpos,     ypos,       0.0, 1.0,
            xpos + w, ypos,       1.0, 1.0,

            xpos,     ypos + h,   0.0, 0.0,
            xpos + w, ypos,       1.0, 1.0,
            xpos + w, ypos + h,   1.0, 0.0
        };

        text_vao.SetData(vertices);

        glBindVertexArray(text_vao);
        glBindTexture(GL_TEXTURE_2D, character.texture_id_);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (character.advance_ >> 6) * scale;
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

void Render::DrawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, const Color& color)
{
    std::array<GLfloat, 12> coords = {
        x - 0.5f*w, y - 0.5f*h,
        x - 0.5f*w, y + 0.5f*h,
        x + 0.5f*w, y + 0.5f*h,

        x - 0.5f*w, y - 0.5f*h,
        x + 0.5f*w, y + 0.5f*h,
        x + 0.5f*w, y - 0.5f*h
    };

    simple_program_.SetParam("u_color", color.r, color.g, color.b, color.a);
    simple_vao.SetData(coords);

    glUseProgram(simple_program_);
    glBindVertexArray(simple_vao);
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

    std::array<GLfloat, 12> coords = {
        x0 + n.x, y0 + n.y,
        x0 - n.x, y0 - n.y,
        x1 + n.x, y1 + n.y,
        x0 + n.x, y0 + n.y,
        x1 + n.x, y1 + n.y,
        x1 - n.x, y1 - n.y
    };

    simple_program_.SetParam("u_color", color.r, color.g, color.b, color.a);
    simple_vao.SetData(coords);

    glUseProgram(simple_program_);
        glBindVertexArray(simple_vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
}

void Render::DrawUnit(const ::Unit &unit, GLfloat scale)
{
    const auto& unit_ = units_.at(unit.Type());

    DrawTexture(unit_.texture_id_, unit.Geometry().position.x, unit.Geometry().position.y, unit_.width_*scale, unit_.height_*scale, unit.Geometry().rotation);
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
}

std::array<GLfloat, 24> Render::GetVertices(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat a)
{
    auto lt = glm::rotate(glm::vec2{-0.5*w, +0.5f*h}, glm::radians(a)) + glm::vec2(x, y);
    auto lb = glm::rotate(glm::vec2{-0.5*w, -0.5f*h}, glm::radians(a)) + glm::vec2(x, y);
    auto rb = glm::rotate(glm::vec2{+0.5*w, -0.5f*h}, glm::radians(a)) + glm::vec2(x, y);
    auto rt = glm::rotate(glm::vec2{+0.5*w, +0.5f*h}, glm::radians(a)) + glm::vec2(x, y);

    return {
        lt.x, lt.y, 1.0f, 1.0f,
        lb.x, lb.y, 0.0f, 1.0f,
        rb.x, rb.y, 0.0f, 0.0f,

        lt.x, lt.y, 1.0f, 1.0f,
        rb.x, rb.y, 0.0f, 0.0f,
        rt.x, rt.y, 1.0f, 0.0f
    };
}

void Render(class Render& render, const ammo::Shot& shot)
{
    render.DrawUnit(shot);
}

void Render(class Render& render, const ammo::LaserRay& ray)
{
    render.DrawLine(ray.Geometry().position.x,
                    ray.Geometry().position.y,
                    ray.Enemy()->Geometry().position.x,
                    ray.Enemy()->Geometry().position.y,
                    Color{1.0, 0.0, 0.0, 0.7f});
}

void Render(class Render& render, const ammo::Rocket& rocket)
{
    if(rocket.Type() == UnitType::Boom)
        render.DrawUnit(rocket, std::min(rocket.BoomTime(), rocket.MaxBoomTime())*30.0f);
    else
        render.DrawUnit(rocket);
}

void Render(class Render& render, const tower::Gun& gun)
{
    for(auto&& ammo: gun)
        Render(render, ammo);

    render.DrawUnit(gun);
}

void Render(class Render& render, const tower::Laser& laser)
{
    const auto& ray = laser.Ray();
    if(ray)
        Render(render, *ray);

    render.DrawUnit(laser);
}

void Render(class Render& render, const tower::RocketGun& rocket_gun)
{
    for(auto&& ammo: rocket_gun)
        Render(render, ammo);

    render.DrawUnit(rocket_gun);
}

void Render(class Render& render, const tower::TowerBase& tower)
{
    switch (tower.Type())
    {
        case UnitType::Gun:
            Render(render, static_cast<const tower::Gun&>(tower));
            break;
        case UnitType::Laser:
            Render(render, static_cast<const tower::Laser&>(tower));
            break;
        case UnitType::RocketGun:
            Render(render, static_cast<const tower::RocketGun&>(tower));
            break;
        default:
            assert(false && "Invalid tower type");
    }
}

void Render(class Render& render, const Map& map)
{
    render.DrawUnit(map);

    for(auto&& enemy: map.Enemies())
    {
        render.DrawUnit(*enemy);
    }

    for(auto&& tower: map.Towers())
    {
        ::Render(render, *tower);
    }

    std::string level(9, '\0');
    sprintf(level.data(), "Level: %2d", map.Level());
    render.DrawText(level, 640.0f, 440.0f, {0.5, 0.8f, 0.2f, 1.0f});

    std::string Health(10, '\0');
    sprintf(Health.data(), "Health: %2d", map.Health());
    render.DrawText(Health, 640.0f, 410.0f, {0.5, 0.8f, 0.2f, 1.0f});


    std::string Gold(10, '\0');
    sprintf(Gold.data(), "Gold: %4d", map.Gold());
    render.DrawText(Gold, 640.0f, 380.0f, {0.5, 0.8f, 0.2f, 1.0f});

    std::string text(11, '\0');
    sprintf(text.data(), "Wave: %5.2f", map.TimeOffset());
    render.DrawText(text, 640.0f, 350.0f, {0.5, 0.8f, 0.2f, 1.0f});
}
