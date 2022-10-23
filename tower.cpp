#include "tower.h"

#include "render.h"

Tower::Tower(UnitType type, Point pos, Degree angle) :
    Unit{ type, pos, angle }
{
    switch (type) {
    case UnitType::Gun:
        radius_ = 100.0f;
        max_reloading_ = 0.5f;
        break;
    case UnitType::Laser:
        radius_ = 150.0f;
        max_reloading_ = 3.0f;
        break;
    case UnitType::RocketGun:
        radius_ = 250.0f;
        max_reloading_ = 7.0f;
        break;
    default:
        break;
    }
}

void Tower::Render(class Render &render)
{
    for(auto&& ammo : ammo_)
    {
        ammo->Render(render);
    }

    render.DrawUnit(*this);
}

Ammo::Ammo(UnitType type, Point position, Degree rotation, ID enemy_id) :
    Unit{ type, position, rotation }, enemy_id_{enemy_id}, destroyed_{}
{

}

bool Ammo::IsDestroyed() const noexcept
{
    return destroyed_;
}

ID Ammo::EnemyID() const noexcept
{
    return enemy_id_;
}

void Ammo::Render(class Render &render)
{
    render.DrawUnit(*this);
}

Shot::Shot(Point position, Degree rotation, ID enemy_id) :
    Ammo{ UnitType::Shot, position, rotation, enemy_id }
{

}

void Shot::DestroyImpl(float time)
{
    destroyed_ = true;
}

void Shot::MoveImpl(Enemy &enemy, float time)
{
    auto direction = glm::normalize(enemy.Position() - position_);

    rotation_ = glm::sign(-direction.y)*glm::degrees(glm::angle(direction, glm::vec2{ 1.0f, 0.0f }));

    position_ += direction*SPEED*time;

    if(glm::distance(enemy.Position(), position_) < 1.5f)
    {
        enemy.Damage(DAMAGE);
        DestroyImpl(0.0f);
    }
}

LaserRay::LaserRay(Point position, Degree rotation, ID enemy_id) :
    Ammo{ UnitType::LaserRay, position, rotation, enemy_id }, target_position_{position}, life_time_{}
{

}

void LaserRay::Render(class Render& render)
{
    render.DrawLine(position_.x, position_.y, target_position_.x, target_position_.y, Color{1.0, 0.0, 0.0, 0.7f});
}

void LaserRay::DestroyImpl(float time)
{
    destroyed_ = true;
}

void LaserRay::MoveImpl(Enemy &enemy, float time)
{
    auto direction = glm::normalize(enemy.Position() - position_);

    rotation_ = glm::sign(-direction.y)*glm::degrees(glm::angle(direction, glm::vec2{ 1.0f, 0.0f }));

    enemy.Damage(DAMAGE*time);

    life_time_ += time;

    target_position_ = enemy.Position();

    if(life_time_ > MAX_LIFE_TIME)
    {
        DestroyImpl(0.0f);
    }
}

Rocket::Rocket(Point position, Degree rotation, ID enemy_id) :
    Ammo{ UnitType::Rocket, position, rotation, enemy_id }, boom_time_{}, is_boom_{}
{

}

void Rocket::Render(class Render &render)
{
    if(is_boom_)
    {
        Unit boom{ UnitType::Boom, position_, 0.0f };
        render.DrawUnit(boom, std::min(MAX_BOOM_TIME, boom_time_)*30.0f);
    }
    else
    {
        render.DrawUnit(*this);
    }
}

void Rocket::DestroyImpl(float time)
{
    is_boom_ = true;
    boom_time_ += BOOM_SPEED*time;
    if(boom_time_ > MAX_BOOM_TIME)
    {
        destroyed_ = true;
    }
}

void Rocket::MoveImpl(Enemy &enemy, float time)
{
    if(is_boom_)
    {
        DestroyImpl(time);
        return;
    }

    auto direction = glm::normalize(enemy.Position() - position_);

    rotation_ = glm::sign(-direction.y)*glm::degrees(glm::angle(direction, glm::vec2{ 1.0f, 0.0f }));

    position_ += direction*SPEED*time;

    if(glm::distance(enemy.Position(), position_) < 1.5f)
    {
        is_boom_ = true;
        enemy.Damage(DAMAGE);
        DestroyImpl(0.0f);
    }
}
