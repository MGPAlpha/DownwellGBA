#include "enemy.hpp"

#include <cstdlib>

#include "player.hpp"
#include "gem.hpp"
#include "camera.hpp"
#include "spritedata.hpp"
#include "collision.hpp"

using namespace GBAEngine;

int enemiesKilled = 0;

void Enemy::update() {
    
    if (Player* player = Player::getSingleton()) {
        int playerDist = player->getCollider().pos.y - this->collider.pos.y;
        if (playerDist > this->maxPlayerRange) this->getGameObject()->destroy();
    }
}

void Enemy::draw() {
    OBJ_ATTR *sprite = this->sprite;
    int posX = (this->collider.pos.x) + this->spriteOffset.x - cameraPos.x;
    int posY = (this->collider.pos.y) + this->spriteOffset.y - cameraPos.y;

    if (posY < -16 || posY > 160 || posX < -32 || posX > 240) {
        sprite->attr0 = ATTR0_HIDE;
        return;
    }

    sprite->attr0 = ATTR0_REGULAR | ATTR0_WIDE | posY & 0x00ff;
    sprite->attr1 = ATTR1_MEDIUM | posX & 0x01ff;
    sprite->attr2 = this->calculateSpriteIndex() | ATTR2_PRIORITY(3);

}

Enemy::Enemy(Vector2 pos) {
    this->collider.pos.x = pos.x;
    this->collider.pos.y = pos.y;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->frameExtraMovement.x = 0;
    this->frameExtraMovement.y = 0;
    this->dead = 0;
}

BlobEnemy::BlobEnemy(Vector2 pos) : Enemy(pos) {
    this->health = 3;
    this->spriteOffset = Vector2(-7, -3);
    this->collider.size = Vector2(18, 11);
    this->collider.pos = this->collider.pos - this->collider.size / 2;
    this->maxPlayerRange = 800;
}

int BlobEnemy::calculateSpriteIndex() {
    return ATTR2_TILEID(this->getGameObject()->getLifetime() / 6 % 4 * 4,16);
}

void BlobEnemy::update() {
    Enemy::update();
    if (Player* player = Player::getSingleton()) {
        
        Rect playerCollider = player->getCollider();
        Vector2 playerPos = playerCollider.pos;
        playerPos.x += playerCollider.size.x/2;
        playerPos.y += playerCollider.size.y/2;

        Vector2 enemyPos = this->collider.pos;
        enemyPos.x += this->collider.size.x/2;
        enemyPos.y += this->collider.size.y/2;

        Vector2 displacement = playerPos - enemyPos;
        displacement = displacement.normalized();
        displacement = displacement / 64;

        this->velocity.x += displacement.x;
        this->velocity.y += displacement.y;

        if (this->velocity.x.value > 1<<15) this->velocity.x.value = 1<<15;
        if (this->velocity.x.value < -1<<15) this->velocity.x.value = -1<<15;
        if (this->velocity.y.value > 1<<15) this->velocity.y.value = 1<<15;
        if (this->velocity.y.value < -1<<15) this->velocity.y.value = -1<<15;

    }


    this->collider.pos.x += this->velocity.x + this->frameExtraMovement.x;
    Collision terrainCollision = collideCollisionMap(this->collider, activeCollisionMap, activeCollisionMapWidth, 20);
    if (terrainCollision.collided) {
        this->collider.pos.x += terrainCollision.push.x;
        // this->velocity.x *= -1;
    }
    this->frameExtraMovement.x = 0;

    this->collider.pos.y += this->velocity.y + this->frameExtraMovement.y;
    terrainCollision = collideCollisionMap(this->collider, activeCollisionMap, activeCollisionMapWidth, 20);
    if (terrainCollision.collided) {
        this->collider.pos.y += terrainCollision.push.y;
        this->velocity.y *= -1;
    }
    this->frameExtraMovement.y = 0;
}

void Enemy::damageEnemy(int damage) {
    this->health -= damage;
    this->velocity.y += 2;
    this->frameExtraMovement.y = 2;
    if (this->health <= 0) {
        this->killEnemy();
    }
}

void Enemy::killEnemy() {
    Vector2 gemSpawn = this->collider.pos + this->collider.size / 2;
    int gemCount = randRange(2,4);
    for (int i = 0; i < gemCount; i++){
        GameObject *newGem = new GameObject();
        newGem->addComponent(new Gem(gemSpawn));
        GameObject::loadGameObject(newGem);
    }
    enemiesKilled++;
    this->getGameObject()->destroy();
}

GameObjectGenerator blobGenerator = GameObjectGenerator([](Vector2 pos) {
    GameObject* go = new GameObject();
    go->addComponent(new BlobEnemy(pos));
    return go;
});
