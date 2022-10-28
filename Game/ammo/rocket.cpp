#include "rocket.h"

#include <enemy/enemy.h>

namespace bugattack::ammo
{

Rocket::Rocket(class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    AmmoBase{UnitType::Rocket, geometry, target}, boom_time_{}
{

}

void Rocket::Update(float time)
{
    if(type_ == UnitType::Boom)
    {
        boom_time_ += BOOM_SPEED*time;
        return;
    }

    geometry_ = MoveTo(geometry_, target_->Geometry(), SPEED*time);

    if(Distance(geometry_, target_->Geometry()) < DAMAGE_DISTANCE)
    {
        target_->Damage(DAMAGE);
        type_ = UnitType::Boom;
    }
}

bool Rocket::IsDestroyed() const
{
    return boom_time_ > MAX_BOOM_TIME;
}

float Rocket::BoomTime() const noexcept
{
    return boom_time_;
}

float Rocket::MaxBoomTime() const noexcept
{
    return MAX_BOOM_TIME;
}

}
