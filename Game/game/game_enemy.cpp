#include "game_enemy.h"

namespace bugattack
{

void Update(GameEnemy& enemy, const field::Field& field, float time)
{
    enemy.game_enemy_->Update(field, time);
}

int Cost(GameEnemy& enemy)
{
    return enemy.game_enemy_->Cost();
}

bool InEnd(GameEnemy& enemy, const field::Field& field)
{
    return enemy::InEnd(field, *enemy);
}

}
