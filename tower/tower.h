#pragma once

#include <vector>
#include <memory>

#include <enemy.h>
#include <unit.h>

namespace tower
{

class TowerBase : public Unit
{
public:
    TowerBase(UnitType type, class Geometry geometry);

    virtual void Update(const std::vector<std::shared_ptr<Enemy>>& enemies, float time) = 0;

    virtual float Radius() const = 0;

    virtual ~TowerBase() = default;
protected:
    float reloading_;
};

}
