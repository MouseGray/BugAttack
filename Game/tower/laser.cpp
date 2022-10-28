#include "laser.h"

#include <enemy/enemy.h>

namespace bugattack::tower
{

Laser::Laser(class Geometry geometry) :
    TowerBase{UnitType::Laser, geometry}
{

}

void Laser::Update(const std::vector<std::shared_ptr<enemy::Enemy>>& enemies, float time)
{
    reloading_ -= time;

    if(ammo_)
    {
        ammo_->Update(time);
        if(ammo_->IsDestroyed())
            ammo_.reset();
    }

    auto min_distance = RADIUS;
    std::shared_ptr<enemy::Enemy> target_enemy = nullptr;

    if(ammo_)
    {
        target_enemy = ammo_->Target();
    }
    else
    {
        for(auto&& enemy: enemies)
        {
            auto distance = Distance(geometry_, enemy->Geometry());
            if(distance < min_distance)
            {
                min_distance = distance;
                target_enemy = enemy;
            }
        }
    }

    if(!target_enemy)
        return;

    geometry_ = LookAt(geometry_, target_enemy->Geometry());

    if(reloading_ <= 0.0f)
    {
        ammo_.emplace(geometry_, std::move(target_enemy));
        reloading_ = MAX_RELOAD;
    }
}

float Laser::Radius() const
{
    return RADIUS;
}

const std::optional<ammo::LaserRay>& Laser::Ray() const noexcept
{
    return ammo_;
}

}
