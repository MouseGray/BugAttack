#include "rocket.h"

#include <enemy/enemy.h>

namespace bugattack::ammo
{

Rocket::Rocket(class Geometry geometry, std::shared_ptr<enemy::Enemy> target) :
    AmmoBase{UnitType::Rocket, std::move(geometry), target}, boom_time_{}
{

}

bool Rocket::IsBoom() const noexcept
{
    return type_ == UnitType::Boom;
}

void Rocket::Boom() noexcept
{
    assert(!IsBoom() && "The rocket is already blown up");

    type_ = UnitType::Boom;
}

void Rocket::AddBoomTime(float time) noexcept
{
    assert(IsBoom() && "The rocket is not blown up");

    boom_time_ += BOOM_SPEED*time;
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

float Rocket::Damage() const
{
    return DAMAGE;
}

float Rocket::Velocity() const
{
    return VELOCITY;
}

void Update(Rocket& rocket, float time) noexcept
{
    if(rocket.IsBoom())
    {
        rocket.AddBoomTime(time);
        return;
    }

    rocket.SetGeometry(MoveTo(rocket.Geometry(), rocket.Target().Geometry(), rocket.Velocity()*time));

    if(Distance(rocket.Geometry(), rocket.Target().Geometry()) < Rocket::DAMAGE_DISTANCE)
    {
        rocket.Target().Damage(rocket.Damage());
        rocket.Boom();
    }
}

}
