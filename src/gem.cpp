#include "gem.hpp"

#include <cstdlib>

#include "player.hpp"
#include "savedata.hpp"
#include "engine/camera.hpp"
#include "collision.hpp"

extern "C" {

#include "print.h"

}

using namespace GBAEngine;

int gemsHeld = 0;
int totalGemsThisRun = 0;

int lifetimeGems = 0;

void initGemData(void) {
    gemsHeld = 0;
    totalGemsThisRun = 0;
    lifetimeGems = getSaveInt(8);
}

Gem::Gem() {
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->state = GEM_PHYSICS;
    this->stateTime = 0;
    this->type = GEM_SMALL;
    this->rotationDirection = GEM_CCW;
}

void Gem::awake() {
    this->transform = getComponent<Transform>();
    this->collider = getComponent<RectCollider>();
    this->randomizeGem();
}

void Gem::update() {
    switch (this->state) {

        case GEM_PHYSICS:
            {
                this->velocity.y.value += 8<<8;

                this->transform->position.x += this->velocity.x;
                Collision xColl = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
                if (xColl.collided) {
                    this->transform->position.x += xColl.push.x;
                    this->velocity.x = -this->velocity.x;
                }
                this->transform->position.y += this->velocity.y;
                Collision yColl = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
                if (yColl.collided) {
                    this->transform->position.y += yColl.push.y;
                    if (this->velocity.y >= 0) this->velocity.y = -1;
                    else this->velocity.y = -this->velocity.y;
                }

                if (Player* player = Player::getSingleton()) {
                    Vector2 playerCenter = player->getTransform()->position + player->getCollider()->size / 2;
                    Vector2 gemCenter = this->transform->position + this->collider->size / 2;
                    Vector2 playerDisp = playerCenter - gemCenter;
                    if (playerDisp.magnitude() < 18) {
                        this->state = GEM_ATTRACT;
                        this->stateTime = 0;
                    }

                }

                if (this->getGameObject()->getLifetime() > 5*60) {
                    this->getGameObject()->destroy();
                    return;
                }
                break;
            }
        case GEM_ATTRACT:
            {
                if (Player* player = Player::getSingleton()) {
                    Vector2 playerCenter = player->getTransform()->position + player->getCollider()->size / 2;
                    Vector2 gemCenter = this->transform->position + this->collider->size / 2;
                    Vector2 playerDisp = playerCenter - gemCenter;
                    fixed32 playerDistance = playerDisp.magnitude();
                    Vector2 accel = playerDisp.normalized();
                    accel = accel / 3;
                    int accelRamp = this->stateTime;
                    if (accelRamp) accel = accel * accelRamp;

                    this->transform->position.x += this->velocity.x + accel.x;
                    this->transform->position.y += this->velocity.y + accel.y;

                    Collision collisionWithPlayer = collideRects(player->getCollider()->getRect(), this->collider->getRect());
                    if (collisionWithPlayer.collided) {
                        this->getGameObject()->destroy();
                        collectGems(this->type == GEM_LARGE ? 10 : 2);
                        return;
                    }
                }

                this->stateTime++;
                break;
            }
    }
}

void Gem::draw() {
    return;
    int lifetime = this->getGameObject()->getLifetime();
    int spOffset = this->type == GEM_SMALL ? 2 : 6;
    fixed32 posY = (this->transform->position.y) - cameraPos.y - spOffset;
    fixed32 posX = (this->transform->position.x) - cameraPos.x - spOffset;
    int aniFrame = lifetime/4;
    if (this->rotationDirection == GEM_CW) {
        aniFrame = -aniFrame;
    }
    aniFrame = mod(aniFrame, 4);
    int tileId = this->type == GEM_SMALL ? OFFSET(8,12,32) : OFFSET(0,12,32);
    tileId += aniFrame * (this->type == GEM_SMALL ? 1 : 2);
    if (lifetime > 60*4 && lifetime % 2 || posY < -16 || posY > 160 || posX < -16 || posX > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_SQUARE | int(posY) & 0x00ff;
    this->sprite->attr1 = (this->type == GEM_SMALL ? ATTR1_TINY : ATTR1_SMALL) | int(posX) & 0x01ff;
    this->sprite->attr2 = tileId | ATTR2_PRIORITY(2);
}

void Gem::randomizeGem() {
    Vector2 vel;
    vel.x.value = randRange(-1<<16, 1<<16);
    vel.y.value = randRange(-1<<15, 1<<16);
    vel = vel.normalized();
    vel = vel * fixed32(1.5);
    this->velocity = vel;
    this->rotationDirection = (rand() % 2) ? GEM_CCW : GEM_CW;
    this->type = (rand() % 4) ? GEM_SMALL : GEM_LARGE;
}

GemPrefab::GemPrefab(Vector2 pos) {
    Vector2 actualPos = pos - Vector2(2);
    this->addComponent(new Transform(actualPos));

    this->addComponent(new Gem());
    this->addComponent(new RectCollider(Vector2(4,4), RectCollider::TOP_LEFT));
}
GemPrefab::GemPrefab() : GemPrefab(Vector2()) {}

void collectGems(int gems) {
    gemsHeld += gems;
    totalGemsThisRun += gems;
}