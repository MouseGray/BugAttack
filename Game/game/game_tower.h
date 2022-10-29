#pragma once

#include <memory>

#include <tower/gun.h>
#include <tower/laser.h>
#include <tower/rocket.h>

#include <utils/utils.h>

namespace bugattack
{

struct GameTower
{
    struct IGameTower
    {
        virtual void Update(const std::vector<const enemy::Enemy*>& enemies, float time) = 0;

        virtual const tower::TowerBase& Base() const = 0;

        virtual ~IGameTower() = default;
    };

    template<typename Ty>
    struct Tower : IGameTower
    {
        Tower(Ty tower) :
            tower_{std::move(tower)}
        {

        }

        void Update(const std::vector<const enemy::Enemy*>& enemies, float time) override
        {
            tower::Update(tower_, utils::DereferenceIteratorConst{enemies.begin()},
                                  utils::DereferenceIteratorConst{enemies.end()}, time);
        }

        const tower::TowerBase& Base() const override
        {
            return tower_;
        }

        Ty tower_;
    };

    template<typename Ty>
    GameTower(Ty tower) :
        game_tower_{std::make_unique<Tower<Ty>>(std::move(tower))}
    {

    }

    const tower::TowerBase& operator*() const
    {
        return game_tower_->Base();
    }

    const tower::TowerBase* operator->() const
    {
        return std::addressof(game_tower_->Base());
    }

    std::unique_ptr<IGameTower> game_tower_;
};

template<typename IIt>
void Update(GameTower& tower, IIt begin, IIt end, float time)
{
    std::vector<const enemy::Enemy*> enemies;

    std::transform(begin, end, std::back_inserter(enemies), [](auto&& enemy){
        return std::addressof(enemy);
    });

    tower.game_tower_->Update(enemies, time);
}

}

