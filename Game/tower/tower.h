#pragma once

#include <algorithm>
#include <vector>
#include <memory>

#include <unit/unit.h>

namespace bugattack::enemy{class Enemy;}

namespace bugattack::tower
{

class TowerBase : public Unit
{
public:
    TowerBase(UnitType type, class Geometry geometry);

    void Reload(float time) noexcept;

    virtual float Radius() const = 0;

    virtual float MaxRealodTime() const = 0;

    virtual bool Shoot(std::shared_ptr<enemy::Enemy> target) noexcept;
protected:
    virtual ~TowerBase() = default;

    float reloading_;
};

int Cost(UnitType type) noexcept;

template<typename IIt>
std::shared_ptr<enemy::Enemy> FindTarget(const TowerBase& tower, IIt begin, IIt end)
{
    auto target_it = std::min_element(begin, end, [&tower](auto&& enemy, auto&& target){
        return Distance(tower.Geometry(), enemy.Geometry()) < Distance(tower.Geometry(), target.Geometry());
    });

    if(target_it == end)
        return {};

    if(tower.Radius() < Distance(tower.Geometry(), target_it->Geometry()))
        return {};

    return target_it->Share();
}

}
