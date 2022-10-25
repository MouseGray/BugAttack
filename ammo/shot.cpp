#include "shot.h"

#include <enemy.h>
#include <iostream>

namespace ammo
{

Shot::Shot(class Geometry geometry, std::shared_ptr<class Enemy> target) :
    AmmoBase{UnitType::Shot, geometry, target}
{

}

void Shot::Update(float time)
{
    geometry_ = MoveTo(geometry_, target_->Geometry(), SPEED*time);

    if(Distance(target_->Geometry(), geometry_) < DAMAGE_DISTANCE)
    {
        target_->Damage(DAMAGE);
    }
}

bool Shot::IsDestroyed() const
{
    return Distance(geometry_, target_->Geometry()) < DAMAGE_DISTANCE;
}

}
