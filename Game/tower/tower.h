#pragma once

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

    virtual void Update(const std::vector<std::shared_ptr<enemy::Enemy>>& enemies, float time) = 0;

    virtual float Radius() const = 0;

    virtual ~TowerBase() = default;
protected:
    float reloading_;
};

int Cost(UnitType type) noexcept;

}
