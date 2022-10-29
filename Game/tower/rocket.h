#pragma once

#include <ammo/rocket.h>

#include <enemy/enemy.h>

#include "tower.h"

namespace bugattack::tower
{

class RocketGun final : public TowerBase
{
    static constexpr float RADIUS     = 300;
    static constexpr float MAX_RELOAD = 5.0f;
public:
    RocketGun(class Geometry geometry);

    float MaxRealodTime() const override;

    float Radius() const override;

    bool Shoot(std::shared_ptr<enemy::Enemy> target) noexcept override;

    void ValidateAmmo();

    std::vector<ammo::Rocket>::iterator begin() noexcept;

    std::vector<ammo::Rocket>::iterator end() noexcept;

    std::vector<ammo::Rocket>::const_iterator begin() const noexcept;

    std::vector<ammo::Rocket>::const_iterator end() const noexcept;
private:
    std::vector<ammo::Rocket> ammo_;
};

template<typename IIt>
void Update(RocketGun& rocket_gun, IIt begin, IIt end, float time)
{
    rocket_gun.Reload(time);

    std::for_each(rocket_gun.begin(), rocket_gun.end(), [time](auto&& ammo){
        ammo::Update(ammo, time);
    });

    rocket_gun.ValidateAmmo();

    auto target = FindTarget(rocket_gun, begin, end);

    if(!target)
        return;

    rocket_gun.SetGeometry(LookAt(rocket_gun.Geometry(), target->Geometry()));

    rocket_gun.Shoot(target);
}

}
