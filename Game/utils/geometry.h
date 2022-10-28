#pragma once

#include <glm/glm.hpp>

#include "primitives.h"

struct Geometry
{
    glm::vec2 position;
    float rotation;
};

PURE Geometry LookAt(const Geometry& from, const Geometry& to) noexcept;

PURE Geometry Move(const Geometry& geometry, float distance) noexcept;

PURE Geometry MoveTo(const Geometry& from, const Geometry& to, float distance) noexcept;

PURE float Distance(const Geometry& from, const Geometry& to) noexcept;

PURE float Distance(const Geometry& from, const glm::vec2& to) noexcept;
