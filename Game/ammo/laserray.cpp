#include "laserray.h"

#include <enemy/enemy.h>

namespace bugattack::ammo
{

LaserRay::LaserRay(class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    AmmoBase{UnitType::LaserRay, std::move(geometry), target}, work_time_{}
{

}

void LaserRay::AddWorkTime(float time) noexcept
{
    work_time_ += time;
}

float LaserRay::Damage() const
{
    return DAMAGE;
}

float LaserRay::Velocity() const
{
    return VELOCITY;
}

bool LaserRay::IsDestroyed() const
{
    return work_time_ > MAX_WORK_TIME;
}

void Update(LaserRay& ray, float time) noexcept
{
    ray.SetGeometry(MoveTo(ray.Geometry(), ray.Target().Geometry(), ray.Velocity()*time));

    ray.Target().Damage(ray.Damage()*time);

    ray.AddWorkTime(time);
}

}
