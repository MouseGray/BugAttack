#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "field.h"
#include "enemy.h"
#include "tower.h"
#include "level.h"

class Render;

class Map
{


public:
    Map(GLuint texture_id, GLfloat x, GLfloat y, GLfloat w, GLfloat h);


    void Update(float time);


    void generate_enemies(UnitType);

    bool CanPut(const glm::vec2& pos, const glm::vec2& size) const noexcept;

    void Put(UnitType type, size_t x, size_t y) noexcept;

    void Render(class Render& render);

    bool IsWin() const noexcept;

    bool IsLost() const noexcept;

    void Restart();
public:
    GLuint texture_id_;
    GLfloat x_;
    GLfloat y_;
    GLfloat w_;
    GLfloat h_;

    int gold_;
    int health_;

    std::list<Enemy> enemies_;

    std::vector<Tower> towers_;

    bugattack::Level level_;

    bugattack::Field field_;
};
