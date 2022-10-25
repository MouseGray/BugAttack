#include "enemy.h"

#include <cassert>
#include <iostream>
#include <ostream>

#include "field.h"
#include "utils.h"
#include "geometry.h"

Enemy::Enemy(UnitType type, class Geometry geometry, int level) :
    Unit{ type, geometry }, in_end_{}
{
    switch (type)
    {
        case UnitType::Bug1:
            velocity_ = 60.0f + level*2.0f;
            health_   = 100 + level*20;
            cost_ = 50;
            break;
        case UnitType::Bug2:
            velocity_ = 40.0f + level*2.0f;
            health_   = 200 + level*40;
            cost_ = 70;
            break;
        case UnitType::Bug3:
            velocity_ = 47.0f + level*3.0f;
            health_   = 150 + level*30;
            cost_ = 90;
            break;
        default:
            assert(false && "Invalid enemy type");
    }
}

void Enemy::Move(const bugattack::Field& field, float seconds)
{
    auto distance = velocity_*seconds;

    while(distance > 0.001f)
    {
        if(in_end_)
            break;

        auto direction = glm::round(utils::Direction(geometry_.rotation));

        auto position = glm::clamp(glm::round(geometry_.position / 20.0f - direction*0.49f), {0, 0}, {31, 31});

        for(auto d : {direction,
                      direction*glm::mat2x2{ 0,  1,
                                            -1,  0},
                      direction*glm::mat2x2{ 0, -1,
                                             1,  0}})
        {
            auto cell = field[position.y + d.y][position.x + d.x];
            if(cell == bugattack::CellType::Path || cell == bugattack::CellType::End)
            {
                class Geometry cell_geometry = {glm::vec2{position.x + d.x, position.y + d.y}*20.0f, 0.0f};
                auto access_distance = std::min(distance, Distance(geometry_, cell_geometry));
                geometry_ = MoveTo(geometry_, cell_geometry, access_distance);
                distance -= access_distance;
                if(cell == bugattack::CellType::End)
                {
                    in_end_ = true;
                }
                break;
            }
        }
    }
}

void Enemy::Damage(float damage) noexcept
{
    health_ -= damage;
}

bool Enemy::IsLive() const noexcept
{
    return health_ > 0;
}

bool Enemy::InEnd() const noexcept
{
    return in_end_;
}
