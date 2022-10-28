#include "enemy.h"

#include <field/field.h>
#include <field/field_utils.h>
#include <utils/utils.h>

namespace bugattack::enemy
{

Enemy::Enemy(UnitType type, class Geometry geometry, int level) :
    Unit{ type, geometry }
{

}

float Enemy::Velocity() const noexcept
{
    return velocity_;
}

void Enemy::Damage(float damage) noexcept
{
    health_ -= damage;
}

bool Enemy::IsDestroyed() const
{
    return health_ <= 0;
}

void Update(Enemy& enemy, const field::Field& field, float seconds)
{
    auto distance = enemy.Velocity()*seconds;

    enemy.SetGeometry(field::Move(field, enemy.Geometry(), distance));
}

int Cost(const Enemy &enemy) noexcept
{
    if(!enemy.IsDestroyed())
        return 0;

    switch (enemy.Type())
    {
    case UnitType::Bug1:
        return 30;
    case UnitType::Bug2:
        return 60;
    case UnitType::Bug3:
        return 100;
    default:
        assert(false && "Invalid enemy type");
    }
    return 0;
}

bool InEnd(const field::Field& field, const Enemy& enemy)
{
    auto end_point = field.EndPoint();

    return utils::InRange(enemy.Geometry().position, glm::vec2{end_point.second, end_point.first}*20.0f, 1.0f);;
}

}
