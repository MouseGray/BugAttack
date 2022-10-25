#include "rocket.h"

#include <algorithm>

namespace tower
{

RocketGun::RocketGun(class Geometry geometry) :
    TowerBase{UnitType::RocketGun, geometry}
{

}

void RocketGun::Update(const std::vector<std::shared_ptr<Enemy>> &enemies, float time)
{
    reloading_ -= time;

    auto update_rocket = [time](ammo::Rocket& shot) -> void {
        shot.Update(time);
    };

    std::for_each(ammo_.begin(), ammo_.end(), update_rocket);

    ammo_.erase(std::remove_if(ammo_.begin(), ammo_.end(), IsDestroyed{}), ammo_.end());

    auto min_distance = RADIUS;
    std::shared_ptr<Enemy> target_enemy = nullptr;

    for(auto&& enemy: enemies)
    {
        auto distance = Distance(geometry_, enemy->Geometry());
        if(distance < min_distance)
        {
            min_distance = distance;
            target_enemy = enemy;
        }
    }

    if(!target_enemy)
        return;

    geometry_ = LookAt(geometry_, target_enemy->Geometry());

    if(reloading_ <= 0.0f)
    {
        ammo_.emplace_back(geometry_, std::move(target_enemy));
        reloading_ = MAX_RELOAD;
    }
}

float RocketGun::Radius() const
{
    return RADIUS;
}

std::vector<ammo::Rocket>::const_iterator RocketGun::begin() const noexcept
{
    return ammo_.begin();
}

std::vector<ammo::Rocket>::const_iterator RocketGun::end() const noexcept
{
    return ammo_.end();
}

}
