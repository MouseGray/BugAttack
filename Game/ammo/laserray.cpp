#include "laserray.h"

#include <enemy/enemy.h>

namespace bugattack::ammo
{

LaserRay::LaserRay(class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    AmmoBase{UnitType::LaserRay, geometry, target}, work_time_{}
{

}

void LaserRay::Update(float time)
{
    geometry_ = MoveTo(geometry_, target_->Geometry(), SPEED*time);

    target_->Damage(DAMAGE*time);

    work_time_ += time;
}

bool LaserRay::IsDestroyed() const
{
    return work_time_ > MAX_WORK_TIME;
}

}
