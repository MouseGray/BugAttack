#pragma once

#include <list>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "field.h"
#include "enemy.h"
#include "tower.h"

class Render;

class Map
{


public:
    Map(GLuint texture_id, GLfloat x, GLfloat y, GLfloat w, GLfloat h);

    auto begin()
    {
        return enemies_.begin();
    }

    auto end()
    {
        return enemies_.end();
    }

    auto begin_tower()
    {
        return towers_.begin();
    }

    auto end_tower()
    {
        return towers_.end();
    }

    void Update(float time);


    void generate_enemies(int);

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

    int level_;
    int gold_;

    float wave_;
    float offset_;
    int count;

    int health_;



    std::list<Enemy> enemies_;

    std::vector<Tower> towers_;

    bugattack::Field field_;
};
