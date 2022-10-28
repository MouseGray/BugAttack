#pragma once

#include <algorithm>
#include <numeric>

#include <unit/unit.h>

namespace bugattack::field{class Field;}

namespace bugattack::enemy
{

class Enemy : public Unit, public IDestroyable
{
public:
    Enemy(UnitType type, class Geometry geometry, int level);

    float Velocity() const noexcept;

    void Damage(float damage) noexcept;

    bool IsDestroyed() const;
protected:
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

int Cost(const Enemy& enemy) noexcept;

template<typename IIt>
int Cost(IIt begin, IIt end)
{
    return std::accumulate(begin, end, 0, [](int sum, auto&& obj){
        return sum + enemy::Cost(*obj);
    });
}

bool InEnd(const field::Field& field, const Enemy& enemy);

template<typename IIt>
int InEnd(IIt begin, IIt end, const field::Field& field)
{
    return std::count_if(begin, end, [&field](auto&& obj){
        return enemy::InEnd(field, *obj);
    });
}

}
