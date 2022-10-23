#pragma once

#include <glm/gtx/vector_angle.hpp>
#include <list>
#include <algorithm>
#include <memory>
#include <vector>

#include "unit.h"
#include "enemy.h"

class Render;

class Ammo : public Unit
{
public:
    Ammo(UnitType type, Point position, Degree rotation, ID enemy_id);

    virtual ~Ammo() = default;

    template<typename IIt>
    void Move(IIt begin, IIt end, float time)
    {
        auto enemy_id = [id = enemy_id_](const Enemy& enemy) -> bool {
            return enemy.EnemyID() == id;
        };

        auto it = std::find_if(begin, end, enemy_id);

        if(it == end)
        {
            DestroyImpl(time);
            return;
        }

        MoveImpl(*it, time);
    }

    bool IsDestroyed() const noexcept;

    ID EnemyID() const noexcept;

    virtual void Render(class Render& render);
protected:
    virtual void DestroyImpl(float time) = 0;
    virtual void MoveImpl(Enemy& enemy, float time) = 0;

    ID enemy_id_;
    uint8_t destroyed_: 1;
};

class Shot : public Ammo
{
    static constexpr uint32_t DAMAGE = 10;
    static constexpr float SPEED = 150.0f;
public:
    Shot(Point position, Degree rotation, ID enemy_id);
private:
    void DestroyImpl(float time) override;
    void MoveImpl(Enemy &enemy, float time) override;
};

class LaserRay : public Ammo
{
    static constexpr uint32_t DAMAGE = 50;
    static constexpr float MAX_LIFE_TIME = 1.0f;
public:
    LaserRay(Point position, Degree rotation, ID enemy_id);

    void Render(class Render& render) override;
private:
    void DestroyImpl(float time) override;
    void MoveImpl(Enemy &enemy, float time) override;

    Point target_position_;
    float life_time_;
};

class Rocket : public Ammo
{
    static constexpr uint32_t DAMAGE = 70;
    static constexpr float SPEED = 100.0f;
    static constexpr float MAX_BOOM_TIME = 1.5f;
    static constexpr float BOOM_SPEED = 7.0f;
public:
    Rocket(Point position, Degree rotation, ID enemy_id);

    void Render(class Render& render) override;
private:
    void DestroyImpl(float time) override;
    void MoveImpl(Enemy &enemy, float time) override;

    float boom_time_;
    uint8_t is_boom_: 1;
};



struct Tower : Unit
{
    Tower(UnitType type, Point pos, Degree angle);

    inline auto begin() { return ammo_.begin(); }
    inline auto end() { return ammo_.end(); }

    template<typename IIt>
    void Attack(IIt begin, IIt end, float time)
    {
        reloading_ -= time;

        auto move_ammo = [begin, end, time](std::unique_ptr<Ammo>& ammo) -> void {
            ammo->Move(begin, end, time);
        };

        auto is_destroyed = [](const std::unique_ptr<Ammo>& ammo) -> bool {
            return ammo->IsDestroyed();
        };

        std::for_each(ammo_.begin(), ammo_.end(), move_ammo);

        ammo_.erase(std::remove_if(ammo_.begin(), ammo_.end(), is_destroyed), ammo_.end());

        auto min_radius = radius_;
        Enemy* target_enemy = nullptr;

        if(type_ == UnitType::Laser && !ammo_.empty())
        {
            for(; begin != end; ++begin)
            {
                if(begin->EnemyID() == ammo_.front()->EnemyID())
                {
                     target_enemy = std::addressof(*begin);
                     break;
                }
            }
        }
        else
        {
            for(; begin != end; ++begin)
            {
                auto distance = glm::distance(position_, begin->Position());
                if(distance < min_radius)
                {
                    min_radius = distance;
                    target_enemy = std::addressof(*begin);
                }
            }
        }



        if(!target_enemy)
            return;

        rotation_ = glm::sign(-(target_enemy->Position() - position_).y)*
                glm::degrees(glm::angle(glm::normalize(target_enemy->Position() - position_), glm::vec2{ 1.0f, 0.0f }));

        if(reloading_ <= 0.0f)
        {
            if(type_ == UnitType::RocketGun)
            {
                ammo_.push_back(std::make_unique<Rocket>(position_, rotation_, target_enemy->EnemyID()));
            }
            else if(type_ == UnitType::Laser)
            {
                ammo_.push_back(std::make_unique<LaserRay>(position_, rotation_, target_enemy->EnemyID()));
            }
            else if(type_ == UnitType::Gun)
            {
                ammo_.push_back(std::make_unique<Shot>(position_, rotation_, target_enemy->EnemyID()));
            }
            reloading_ = max_reloading_;
        }
    }

    void Render(class Render& render);
private:
    std::vector<std::unique_ptr<Ammo>> ammo_;

    float radius_;

    float reloading_;

    float max_reloading_;
};
