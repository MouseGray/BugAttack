#include "field_utils.h"

#include <algorithm>

#include <utils/utils.h>

namespace bugattack::field
{

glm::vec2 ToFieldPoint(const Geometry& geometry) noexcept
{
    auto raw_point = glm::round(geometry.position/POINT_SIZE);

    if(utils::IsEqual(geometry.position, raw_point*POINT_SIZE))
        return glm::clamp(raw_point, {0.0f, 0.0f}, {31.0f, 31.0f});

    auto raw_point_direction = glm::round(utils::Direction(geometry.position, raw_point*20.0f));

    auto object_direction = glm::round(utils::Direction(geometry.rotation));

    if(!utils::IsCodirectional(raw_point_direction, object_direction))
        return glm::clamp(raw_point, {0.0f, 0.0f}, {31.0f, 31.0f});

    return glm::clamp(raw_point - object_direction, {0.0f, 0.0f}, {31.0f, 31.0f});
}

Geometry FromFieldPoint(const glm::ivec2& point) noexcept
{
    return {glm::vec2{point}*20.0f, 0.0f};
}

Geometry Move(const Field& field, Geometry geometry, float distance) noexcept
{
    constexpr auto inc_90deg = glm::mat2x2{ 0.0f, 1.0f,
                                           -1.0f, 0.0f};

    constexpr auto dec_90deg = glm::mat2x2{0.0f, -1.0f,
                                           1.0f,  0.0f};

    while(distance > 0.001f)
    {
        if(utils::InRange(geometry.position, glm::vec2{field.EndPoint().second, field.EndPoint().first}*POINT_SIZE, 1.0f))
            return geometry;

        auto dir = glm::round(utils::Direction(geometry.rotation));

        auto pos = ToFieldPoint(geometry);

        auto dirs = {pos + dir, pos + dir*inc_90deg, pos + dir*dec_90deg};

        auto delta = std::find_if(dirs.begin(), dirs.end(), [&field](auto&& dir)
        {
            return IsPath(field, dir.x, dir.y);
        });

        if(delta == dirs.end())
            return geometry;

        auto access_distance = std::min(distance, Distance(geometry, FromFieldPoint(*delta)));

        geometry = MoveTo(geometry, FromFieldPoint(*delta), access_distance);

        distance -= access_distance;
    }
    return geometry;
}

}
