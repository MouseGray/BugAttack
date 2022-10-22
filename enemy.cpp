#include "enemy.h"

#include <cassert>

#include "field.h"

Enemy::Enemy(UnitType type, Point pos, Degree angle, int level) :
    Unit{ type, pos, angle }, id_{ Enemy::GetID() }
{
    switch (type)
    {
        case UnitType::Bug1:
            velocity_ = 60.0f + level*2.0f;
            health_   = 100 + level*20;
            break;
        case UnitType::Bug2:
            velocity_ = 40.0f + level*2.0f;
            health_   = 200 + level*40;
            break;
        case UnitType::Bug3:
            velocity_ = 47.0f + level*3.0f;
            health_   = 150 + level*30;
            break;
        default:
            assert(false && "Invalid enemy type");
    }
}

void Enemy::Move(const bugattack::Field& field, Time seconds)
{
    auto distance = velocity_*seconds;

    while(distance > 0.001f)
    {
        if(in_end_)
            break;

        float access_distance = glm::min(20.0f, distance);

        int dx = glm::round(glm::cos(glm::radians(rotation_)));
        int dy = -glm::round(glm::sin(glm::radians(rotation_)));

        int x = glm::clamp<int>(glm::round(position_.x / 20.0f - dx*0.5f), 0, 31);
        int y = glm::clamp<int>(glm::round(position_.y / 20.0f - dy*0.5f), 0, 31);

        if(field[y + dy][x + dx] == bugattack::CellType::Path ||
                field[y + dy][x + dx] == bugattack::CellType::End)
        {
            auto offset = glm::vec2{dx, dy}*access_distance;
            distance -= offset.length();
            position_ += offset;
            if(field[y + dy][x + dx] == bugattack::CellType::End)
            {
                in_end_ = true;
            }
            continue;
        }

        rotation_ += 90.0f;

        dx = glm::round(glm::cos(glm::radians(rotation_)));
        dy = -glm::round(glm::sin(glm::radians(rotation_)));

        if(field[y + dy][x + dx] == bugattack::CellType::Path ||
                field[y + dy][x + dx] == bugattack::CellType::End)
        {
            auto offset = glm::vec2{dx, dy}*access_distance;
            distance -= offset.length();
            position_ += offset;
            if(field[y + dy][x + dx] == bugattack::CellType::End &&
                    (position_ - Point{(x + dx)*20.0f, (y + dy)*20.0f}).length() < 0.1f)
                in_end_ = true;
            continue;

        }

        rotation_ -= 180.0f;
        dx = glm::round(glm::cos(glm::radians(rotation_)));
        dy = -glm::round(glm::sin(glm::radians(rotation_)));

        if(field[y + dy][x + dx] == bugattack::CellType::Path ||
                field[y + dy][x + dx] == bugattack::CellType::End)
        {
            auto offset = glm::vec2{dx, dy}*access_distance;
            distance -= offset.length();
            position_ += offset;
            if(field[y + dy][x + dx] == bugattack::CellType::End &&
                    (position_ - Point{(x + dx)*20.0f, (y + dy)*20.0f}).length() < 0.1f)
                in_end_ = true;
            continue;

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

ID Enemy::GetID() noexcept
{
    static ID id{};
    return id++;
}
