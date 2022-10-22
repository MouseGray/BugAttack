#pragma once

#include <vector>

#include <GL/glew.h>

#include "primitives.h"

class Render;

class Button
{
    static constexpr Color BODY_COLOR = {0.0f, 0.0f, 0.0f, 1.0f};
    static constexpr Color BORDER_COLOR = {0.6f, 0.6f, 0.6f, 1.0f};
    static constexpr Color BORDER_HOVERED_COLOR = {1.0f, 0.5f, 0.0f, 1.0f};
    static constexpr Color BORDER_SELECTED_COLOR = {1.0f, 0.0f, 0.0f, 1.0f};

public:
    Button(GLuint texture_id, Point position, Size size);

    bool Click(std::size_t x, std::size_t y) noexcept;

    void Hover(std::size_t x, std::size_t y) noexcept;

    void Render(Render& render) const;

    void Disable() noexcept;

    bool IsSelected() const noexcept;
private:
    Color BorderColor() const noexcept;

    bool InArea(std::size_t x, std::size_t y) const noexcept;

    GLuint texture_id_;
    Point position_;
    Size size_;

    unsigned char hovered_: 1;
    unsigned char selected_: 1;
};

class Menu
{
public:
    Menu() = default;

    void MouseClick(std::size_t x, std::size_t y) noexcept;

    void MouseHover(std::size_t x, std::size_t y) noexcept;

    void AddButton(GLuint texture_id, std::size_t x, std::size_t y, std::size_t w, std::size_t h) noexcept;

    void Render(Render& render) const;

    std::size_t Selected() const noexcept;
private:
    std::vector<Button> buttons_;
};

