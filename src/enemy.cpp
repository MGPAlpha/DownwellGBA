#include "enemy.hpp"

#include <cstdlib>

#include "player.hpp"
#include "gem.hpp"
#include "engine/camera.hpp"
#include "engine/sprite.hpp"
#include "spritedata.hpp"
#include "collision.hpp"

using namespace GBAEngine;

int enemiesKilled = 0;

void Enemy::awake() {
    this->transform = getComponent<Transform>();
    this->collider = getComponent<RectCollider>();
}

void Enemy::update() {
    
    if (Player* player = Player::getSingleton()) {
        int playerDist = player->getTransform()->position.y - this->transform->position.y;
        if (playerDist > this->maxPlayerRange) this->getGameObject()->destroy();
    }
}

void Enemy::draw() {
    return;
    OBJ_ATTR *sprite = this->sprite;
    int posX = (this->transform->position.x) /*+ this->spriteOffset.x*/ - cameraPos.x;
    int posY = (this->transform->position.y) /*+ this->spriteOffset.y*/ - cameraPos.y;

    if (posY < -16 || posY > 160 || posX < -32 || posX > 240) {
        sprite->attr0 = ATTR0_HIDE;
        return;
    }

    sprite->attr0 = ATTR0_REGULAR | ATTR0_WIDE | posY & 0x00ff;
    sprite->attr1 = ATTR1_MEDIUM | posX & 0x01ff;
    sprite->attr2 = this->calculateSpriteIndex() | ATTR2_PRIORITY(3);

}

Enemy::Enemy() {
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->frameExtraMovement.x = 0;
    this->frameExtraMovement.y = 0;
    this->dead = 0;
}

BlobEnemy::BlobEnemy() : Enemy() {
    this->health = 3;
    this->spriteOffset = Vector2(-7, -3);
    
    this->maxPlayerRange = 800;
}

void BlobEnemy::awake() {
    Enemy::awake();
    this->transform = getComponent<Transform>();
    this->collider = getComponent<RectCollider>();
}

int BlobEnemy::calculateSpriteIndex() {
    return ATTR2_TILEID(this->getGameObject()->getLifetime() / 6 % 4 * 4,16);
}

void BlobEnemy::update() {
    Enemy::update();
    if (Player* player = Player::getSingleton()) {
        
        Rect playerCollider = player->getCollider()->getRect();
        Vector2 playerPos = playerCollider.pos;
        playerPos.x += playerCollider.size.x/2;
        playerPos.y += playerCollider.size.y/2;

        Vector2 enemyPos = this->transform->position;
        enemyPos = enemyPos + this->collider->size / 2;

        Vector2 displacement = playerPos - enemyPos;
        // displacement = displacement.normalized();
        fixed32 displacementLengthEstimate = fixed32::abs(displacement.x) + fixed32::abs(displacement.y);
        displacement = displacement / displacementLengthEstimate;
        displacement = displacement / 64;

        this->velocity.x += displacement.x;
        this->velocity.y += displacement.y;

        if (this->velocity.x.value > 1<<15) this->velocity.x.value = 1<<15;
        if (this->velocity.x.value < -1<<15) this->velocity.x.value = -1<<15;
        if (this->velocity.y.value > 1<<15) this->velocity.y.value = 1<<15;
        if (this->velocity.y.value < -1<<15) this->velocity.y.value = -1<<15;

    }


    this->transform->position.x += this->velocity.x + this->frameExtraMovement.x;

    Collision terrainCollision = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
    if (terrainCollision.collided) {
        this->transform->position.x += terrainCollision.push.x;
        // this->velocity.x *= -1;
    }
    this->frameExtraMovement.x = 0;

    this->transform->position.y += this->velocity.y + this->frameExtraMovement.y;
    terrainCollision = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
    if (terrainCollision.collided) {
        this->transform->position.y += terrainCollision.push.y;
        this->velocity.y *= -1;
    }
    this->frameExtraMovement.y = 0;
}

BlobPrefab::BlobPrefab(Vector2 pos) {
    Vector2 adjustedPos = pos - Vector2(18, 11)/2;
    this->addComponent(new BlobEnemy());
    this->addComponent(new Transform(pos - Vector2(18,11)/2));
    RectCollider* collider = new RectCollider(Vector2(18,11), RectCollider::CENTER);
    collider->layer = L_2;
    collider->mask = L_0;
    this->addComponent(collider);
    SpriteRenderer* sp = new SpriteRenderer();
    sp->renderPriority = 3;
    this->addComponent(sp);
    this->addComponent(new SpriteAnimator(&Assets::Animations::enmJelly));
};  

void Enemy::damageEnemy(int damage) {
    this->health -= damage;
    this->velocity.y += 2;
    this->frameExtraMovement.y = 2;
    if (this->health <= 0) {
        this->killEnemy();
    }
}

void Enemy::killEnemy() {
    Vector2 gemSpawn = this->transform->position + this->collider->size / 2;
    int gemCount = randRange(2,4);
    for (int i = 0; i < gemCount; i++){
        GemPrefab* newGem = new GemPrefab(gemSpawn);
        GameObject::loadGameObject(newGem);
    }
    enemiesKilled++;
    this->getGameObject()->destroy();
}

GameObjectGenerator blobGenerator = GameObjectGenerator([](Vector2 pos) {
    GameObject* go = new BlobPrefab(pos);
    return go;
});
