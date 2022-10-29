#include "shot.h"

#include <enemy/enemy.h>

namespace bugattack::ammo
{

Shot::Shot(class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    AmmoBase{UnitType::Shot, std::move(geometry), target}
{

}

float Shot::Damage() const
{
    return DAMAGE;
}

float Shot::Velocity() const
{
    return VELOCITY;
}

bool Shot::IsDestroyed() const
{
    return Distance(geometry_, target_->Geometry()) < DAMAGE_DISTANCE;
}

void Update(Shot& shot, float time) noexcept
{
    shot.SetGeometry(MoveTo(shot.Geometry(), shot.Target().Geometry(), shot.Velocity()*time));

    if(Distance(shot.Target().Geometry(), shot.Geometry()) < Shot::DAMAGE_DISTANCE)
    {
        shot.Target().Damage(shot.Damage());
    }
}

}
