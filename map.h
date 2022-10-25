#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include <tower/tower.h>

#include "field.h"
#include "enemy.h"
#include "level.h"

class Render;

class Map : public Unit
{

public:
    Map();

    void Update(float time);

    void GenerateEnemy(UnitType);

    bool CanPut(const glm::vec2& pos, const glm::vec2& size) const noexcept;

    void Put(UnitType type, size_t x, size_t y) noexcept;

    bool IsWin() const noexcept;

    bool IsLost() const noexcept;

    void Restart();

    const std::vector<std::shared_ptr<Enemy>>& Enemies() const noexcept;

    const std::vector<std::unique_ptr<tower::TowerBase>>& Towers() const noexcept;

    int Gold() const noexcept;

    int Health() const noexcept;

    int Level() const noexcept;

    float TimeOffset() const noexcept;
public:
    int gold_;
    int health_;

    std::vector<std::shared_ptr<Enemy>> enemies_;

    std::vector<std::unique_ptr<tower::TowerBase>> towers_;

    bugattack::Level level_;

    bugattack::Field field_;
};
