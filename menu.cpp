#include "menu.h"

#include <algorithm>

#include "render.h"

Button::Button(GLuint texture_id, Point position, Size size) :
    texture_id_{texture_id}, position_{position}, size_{size},
    selected_{}, hovered_{}
{

}

bool Button::Click(std::size_t x, std::size_t y) noexcept
{
    auto in_area = InArea(x, y);
    selected_ ^= in_area;
    return in_area;
}

void Button::Hover(std::size_t x, std::size_t y) noexcept
{
    hovered_ = InArea(x, y);
}

void Button::Render(class Render& render) const
{
    auto std_postion = position_ + 0.5f*size_;

    auto body_size = size_ - Size{4.0f, 4.0f};

    render.DrawRectangle(std_postion.x, std_postion.y, size_.x, size_.y, BorderColor());
    render.DrawRectangle(std_postion.x, std_postion.y, body_size.x, body_size.y, BODY_COLOR);
    render.DrawTexture(texture_id_, std_postion.x, std_postion.y, body_size.x, body_size.y, 270.0f);
}

void Button::Disable() noexcept
{
    selected_ = 0;
}

bool Button::IsSelected() const noexcept
{
    return selected_;
}

Color Button::BorderColor() const noexcept
{
    if(hovered_)
        return BORDER_HOVERED_COLOR;
    if(selected_)
        return BORDER_SELECTED_COLOR;
    return BORDER_COLOR;
}

bool Button::InArea(std::size_t x, std::size_t y) const noexcept
{
    return position_.x <= x && x <= position_.x + size_.x &&
           position_.y <= y && y <= position_.y + size_.y;
}

void Menu::MouseClick(std::size_t x, std::size_t y) noexcept
{
    Button* activated = nullptr;

    for(auto&& btn: buttons_)
    {
        if(btn.Click(x, y))
            activated = std::addressof(btn);
    }

    if(!activated)
        return;

    for(auto&& btn: buttons_)
    {
        if(activated != std::addressof(btn))
            btn.Disable();
    }
}

void Menu::MouseHover(std::size_t x, std::size_t y) noexcept
{
    for(auto&& btn: buttons_)
        btn.Hover(x, y);
}

void Menu::AddButton(GLuint texture_id, std::size_t x, std::size_t y, std::size_t w, std::size_t h) noexcept
{
    buttons_.emplace_back(texture_id, Point{x, y}, Size{w, h});
}

void Menu::Render(class Render& render) const
{
    for(auto&& btn: buttons_)
        btn.Render(render);
}

std::size_t Menu::Selected() const noexcept
{
    auto it = std::find_if(buttons_.begin(), buttons_.end(), [](auto&& btn)
    {
        return btn.IsSelected();
    });

    if(it == buttons_.end())
        return 0;

    return std::distance(buttons_.begin(), it) + 1;
}


