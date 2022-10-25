#include "gun.h"

#include <algorithm>

namespace tower
{

Gun::Gun(class Geometry geometry) :
    TowerBase{UnitType::Gun, geometry}
{

}

void Gun::Update(const std::vector<std::shared_ptr<Enemy>>& enemies, float time)
{
    reloading_ -= time;

    auto update_shot = [time](ammo::Shot& shot) -> void {
        shot.Update(time);
    };

    std::for_each(ammo_.begin(), ammo_.end(), update_shot);

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

float Gun::Radius() const
{
    return RADIUS;
}

std::vector<ammo::Shot>::const_iterator Gun::begin() const noexcept
{
    return ammo_.begin();
}

std::vector<ammo::Shot>::const_iterator Gun::end() const noexcept
{
    return ammo_.end();
}

}
