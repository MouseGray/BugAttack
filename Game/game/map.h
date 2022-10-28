#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <tower/tower.h>
#include <enemy/enemy.h>

#include <field/field.h>
#include <utils/utils.h>

#include "level.h"

namespace bugattack
{

class Map : public Unit
{
    using EnemyIterator = utils::DereferenceIterator<std::vector<std::shared_ptr<enemy::Enemy>>::const_iterator>;
    using TowerIterator = utils::DereferenceIterator<std::vector<std::unique_ptr<tower::TowerBase>>::const_iterator>;

    static constexpr int MAX_HEALTH = 3;
    static constexpr int START_GOLD = 500;
public:
    Map();

    void Update(float time);

    bool CanPut(const glm::vec2& pos) const noexcept;

    void Put(UnitType type, size_t x, size_t y) noexcept;

    void Restart();

    const std::vector<std::shared_ptr<enemy::Enemy>>& Enemies() const noexcept;

    const std::vector<std::unique_ptr<tower::TowerBase>>& Towers() const noexcept;

    EnemyIterator begin_enemy() const noexcept;

    EnemyIterator end_enemy() const noexcept;

    TowerIterator begin_tower() const noexcept;

    TowerIterator end_tower() const noexcept;

    bool IsWin() const noexcept;

    bool IsLost() const noexcept;

    int Gold() const noexcept;

    int Health() const noexcept;

    int Level() const noexcept;

    float TimeOffset() const noexcept;
public:
    void GenerateEnemy(UnitType type);

    int gold_;
    int health_;

    std::vector<std::shared_ptr<enemy::Enemy>> enemies_;

    std::vector<std::unique_ptr<tower::TowerBase>> towers_;

    class Level level_;

    field::Field field_;
};

std::shared_ptr<enemy::Enemy> CreateEnemy(UnitType type, std::size_t x, std::size_t y, int level);

std::unique_ptr<tower::TowerBase> CreateTower(UnitType type, std::size_t x, std::size_t y);

}

