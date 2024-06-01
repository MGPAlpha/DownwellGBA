#include "bullet.hpp"

#include <cstdlib>

#include "engine/gameobject.hpp"
#include "gamestate.hpp"
#include "enemy.hpp"
#include "engine/camera.hpp"
#include "collision.hpp"

extern "C" {

#include "print.h"

}

using namespace GBAEngine;

int bulletTravelFrames[] = {
    8,8,8,8,8,
    7,7,7,
    6,6,6,
    5,5,5,
    4,4,4,
    3,3,3,
    2,2,2,
    1,1,1
};

unsigned int Bullet::CheckCollisionWithEnemy(Enemy *enemy) {

    Collision testCollision = collideRects(this->collider->getRect(), enemy->getComponent<RectCollider>()->getRect());

    if (testCollision.collided) {
        enemy->damageEnemy(1);
        return 1;
    }
    return 0;
}

void Bullet::awake() {
    this->transform = getComponent<Transform>();
    this->collider = getComponent<RectCollider>();
    this->collider->onEnter += [this](Collider* c) {
        if (Enemy* enemy = c->getComponent<Enemy>()) {
            enemy->damageEnemy(1);
            this->getGameObject()->destroy();
        }
    };
}

void Bullet::update() {
    int lifetime = this->getGameObject()->getLifetime();
    int travelFrameIndex = lifetime;
    if (travelFrameIndex >= sizeof(bulletTravelFrames)/(sizeof(int))) travelFrameIndex = sizeof(bulletTravelFrames)/(sizeof(int)) - 1;
    this->transform->position.y += bulletTravelFrames[travelFrameIndex];
    if (lifetime >= 26) {
        this->getGameObject()->destroy();
        return;
    }
    Collision groundCollision = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
    if (groundCollision.collided) {
        this->getGameObject()->destroy();
        return;
    }
    // unsigned int bulletCheckResult = GameObject::doForEachGameObject<Enemy>([this](Enemy* g) {
    //     return this->CheckCollisionWithEnemy(g);
    // });
    // if (bulletCheckResult) {
    //     this->getGameObject()->destroy();
    //     return;
    // }
}

void Bullet::draw() {
    if (!this->sprite) return;
    int lifetime = this->getGameObject()->getLifetime();
    int posY = (this->transform->position.y) - cameraPos.y - 7;
    int posX = (this->transform->position.x) - cameraPos.x - 1;
    if (posY < -16 || posY > 160 || posX < -8 || posX > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_TALL | posY & 0x00ff;
    this->sprite->attr1 = ATTR1_TINY | posX & 0x01ff;
    if (lifetime <= 16) {
        this->sprite->attr2 = ATTR2_TILEID(0,10) | ATTR2_PRIORITY(2);
    } else {
        int aniFrame = (lifetime - 17) / 2 + 1;
        this->sprite->attr2 = ATTR2_TILEID(aniFrame,10) | ATTR2_PRIORITY(2);
    }
}

BulletPrefab::BulletPrefab(Vector2 pos) {
    this->addComponent(new Transform(pos));
    RectCollider* col = new RectCollider(Vector2(6,4), RectCollider::TOP_LEFT);
    col->layer = L_3;
    col->mask = L_1 | L_2;
    this->addComponent(col);
    this->addComponent(new Bullet());
}