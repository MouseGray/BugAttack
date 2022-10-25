#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace utils
{

glm::vec2 Direction(float angle) noexcept;

glm::vec2 Direction(glm::vec2 from, glm::vec2 to) noexcept;

float Angle(glm::vec2 direction, glm::vec2 vector_null = {1.0f, 0.0f});

}
