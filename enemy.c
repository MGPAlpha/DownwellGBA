#include "enemy.h"

int initializeEnemy(GameObject* this) {
    EnemyData *data = malloc(sizeof(EnemyData));
    if (!data) return 1;
    data->collider.size.y = 11<<8;
    data->collider.size.x = 17<<8;
    this->data = data;

    return 0;
}

const GameObjectType enemyType = {
    initializeEnemy,
    NULL,
    NULL,
    NULL
};