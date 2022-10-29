#pragma once

#include <ammo/shot.h>

#include <enemy/enemy.h>

#include "tower.h"

namespace bugattack::tower
{

class Gun final : public TowerBase
{
    static constexpr float RADIUS     = 100;
    static constexpr float MAX_RELOAD = 0.5f;
public:
    Gun(class Geometry geometry);

    bool Shoot(std::shared_ptr<enemy::Enemy> target) noexcept override;

    void ValidateAmmo() noexcept;

    float Radius() const override;

    float MaxRealodTime() const override;

    std::vector<ammo::Shot>::iterator begin() noexcept;

    std::vector<ammo::Shot>::iterator end() noexcept;

    std::vector<ammo::Shot>::const_iterator begin() const noexcept;

    std::vector<ammo::Shot>::const_iterator end() const noexcept;
private:
    std::vector<ammo::Shot> ammo_;
};

template<typename IIt>
void Update(Gun& gun, IIt begin, IIt end, float time)
{
    gun.Reload(time);

    std::for_each(gun.begin(), gun.end(), [time](auto&& ammo){
        ammo::Update(ammo, time);
    });

    gun.ValidateAmmo();

    auto target = FindTarget(gun, begin, end);

    if(!target)
        return;

    gun.SetGeometry(LookAt(gun.Geometry(), target->Geometry()));

    gun.Shoot(target);
}

}
