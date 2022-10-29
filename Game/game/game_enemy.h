#pragma once

#include <memory>

#include <enemy/ant.h>
#include <enemy/ladybug.h>
#include <enemy/cockroach.h>

namespace bugattack
{

struct GameEnemy
{
    struct IGameEnemy
    {
        virtual void Update(const field::Field& field, float time) = 0;

        virtual int Cost() const = 0;

        virtual const enemy::Enemy& Base() const = 0;

        virtual ~IGameEnemy() = default;
    };

    template<typename Ty>
    struct Enemy : IGameEnemy
    {
        Enemy(Ty enemy) :
            enemy_{std::make_shared<Ty>(std::move(enemy))}
        {

        }

        void Update(const field::Field& field, float time) override
        {
            enemy::Update(*enemy_, field, time);
        }

        int Cost() const override
        {
            return enemy::Cost(*enemy_);
        }

        const enemy::Enemy& Base() const override
        {
            return *enemy_;
        }

        std::shared_ptr<Ty> enemy_;
    };

    template<typename Ty>
    GameEnemy(Ty enemy) :
        game_enemy_{std::make_unique<Enemy<Ty>>(std::move(enemy))}
    {

    }

    const enemy::Enemy& operator*() const
    {
        return game_enemy_->Base();
    }

    const enemy::Enemy* operator->() const
    {
        return std::addressof(game_enemy_->Base());
    }

    std::unique_ptr<IGameEnemy> game_enemy_;
};

void Update(GameEnemy& enemy, const field::Field& field, float time);

int Cost(GameEnemy& enemy);

bool InEnd(GameEnemy& enemy, const field::Field& field);

}

