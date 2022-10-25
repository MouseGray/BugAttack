#pragma once

#include <glm/glm.hpp>

struct Geometry
{
    glm::vec2 position;
    float rotation;
};

Geometry LookAt(const Geometry& from, const Geometry& to) noexcept;

Geometry Move(const Geometry& geometry, float distance) noexcept;

Geometry MoveTo(const Geometry& from, const Geometry& to, float distance) noexcept;

float Distance(const Geometry& from, const Geometry& to) noexcept;
