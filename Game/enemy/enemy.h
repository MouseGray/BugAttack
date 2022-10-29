#pragma once

#include <algorithm>
#include <numeric>
#include <memory>

#include <unit/unit.h>

namespace bugattack::field{class Field;}

namespace bugattack::enemy
{

class Enemy : public Unit, public IDestroyable, public std::enable_shared_from_this<Enemy>
{
public:
    Enemy(UnitType type, class Geometry geometry, int level);

    float Velocity() const noexcept;

    void Damage(float damage) noexcept;

    bool IsDestroyed() const;

    virtual std::shared_ptr<Enemy> Share() const = 0;
protected:
    virtual ~Enemy() = default;

    float velocity_;
    float health_;
};

void Update(Enemy& enemy, const field::Field& field, float seconds);

template<typename E>
float Velocity(int level)
{
    return E::BASE_VELOCITY + level*E::COEF_VELOCITY;
}

template<typename E>
float Health(int level)
{
    return E::BASE_HEALTH + level*E::COEF_HEALTH;
}

bool InEnd(const field::Field& field, const Enemy& enemy);

}
