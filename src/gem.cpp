#include "gem.hpp"

#include <cstdlib>

#include "player.hpp"
#include "savedata.hpp"
#include "camera.hpp"
#include "collision.hpp"

extern "C" {

#include "print.h"

}

int gemsHeld = 0;
int totalGemsThisRun = 0;

int lifetimeGems = 0;

void initGemData(void) {
    gemsHeld = 0;
    totalGemsThisRun = 0;
    lifetimeGems = getSaveInt(8);
}

int initializeGem(GameObject* self) {
    GemData *data = (GemData*)malloc(sizeof(GemData));
    if (!data) return 1;
    data->collider.pos.x = 0;
    data->collider.pos.y = 0;
    data->collider.size.x = 4;
    data->collider.size.y = 4;
    data->velocity.x = 0;
    data->velocity.y = 0;
    data->state = GEM_PHYSICS;
    data->stateTime = 0;
    data->type = GEM_SMALL;
    data->rotationDirection = GEM_CCW;
    self->data = data;

    return 0;
}

void updateGem(GameObject* self) {
    GemData *data = (GemData*)self->data;
    
    switch (data->state) {

        case GEM_PHYSICS:
            {
                data->velocity.y.value += 1<<11;

                data->collider.pos.x += data->velocity.x;
                Collision xColl = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 12);
                if (xColl.collided) {
                    data->collider.pos.x += xColl.push.x;
                    data->velocity.x = -data->velocity.x;
                }
                data->collider.pos.y += data->velocity.y;
                Collision yColl = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 12);
                if (yColl.collided) {
                    data->collider.pos.y += yColl.push.y;
                    if (data->velocity.y >= 0) data->velocity.y = -1;
                    else data->velocity.y = -data->velocity.y;
                }

                if (playerSingleton) {
                    PlayerData *playerData = (PlayerData*)playerSingleton->data;
                    Vector2 playerCenter = playerData->collider.pos + playerData->collider.size / 2;
                    Vector2 gemCenter = data->collider.pos + data->collider.size / 2;
                    Vector2 playerDisp = playerCenter - gemCenter;
                    if (playerDisp.magnitude() < 18) {
                        data->state = GEM_ATTRACT;
                        data->stateTime = 0;
                    }

                }

                if (self->lifetime > 5*60) {
                    destroyGameObject(self);
                    return;
                }
                break;
            }
        case GEM_ATTRACT:
            {
                if (playerSingleton) {
                    PlayerData *playerData = (PlayerData*)playerSingleton->data;
                    Vector2 playerCenter = playerData->collider.pos + playerData->collider.size / 2;
                    Vector2 gemCenter = data->collider.pos + data->collider.size / 2;
                    Vector2 playerDisp = playerCenter - gemCenter;
                    fixed32 playerDistance = playerDisp.magnitude();
                    Vector2 accel = playerDisp.normalized();
                    accel = accel / 3;
                    int accelRamp = data->stateTime;
                    if (accelRamp) accel = accel * accelRamp;

                    data->collider.pos.x += data->velocity.x + accel.x;
                    data->collider.pos.y += data->velocity.y + accel.y;

                    Collision collisionWithPlayer = collideRects(playerData->resizedCollider, data->collider);
                    if (collisionWithPlayer.collided) {
                        destroyGameObject(self);
                        collectGems(data->type == GEM_LARGE ? 10 : 2);
                        return;
                    }
                }

                data->stateTime++;
                break;
            }
    }
}

void drawGem(GameObject* self) {
    GemData *data = (GemData*)self->data;
    int spOffset = data->type == GEM_SMALL ? 2 : 6;
    fixed32 posY = (data->collider.pos.y) - cameraPos.y - spOffset;
    fixed32 posX = (data->collider.pos.x) - cameraPos.x - spOffset;
    int aniFrame = self->lifetime/4;
    if (data->rotationDirection == GEM_CW) {
        aniFrame = -aniFrame;
    }
    aniFrame = mod(aniFrame, 4);
    int tileId = data->type == GEM_SMALL ? OFFSET(8,12,32) : OFFSET(0,12,32);
    tileId += aniFrame * (data->type == GEM_SMALL ? 1 : 2);
    if (self->lifetime > 60*4 && self->lifetime % 2 || posY < -16 || posY > 160 || posX < -16 || posX > 240) {
        self->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    self->sprite->attr0 = ATTR0_REGULAR | ATTR0_SQUARE | int(posY) & 0x00ff;
    self->sprite->attr1 = (data->type == GEM_SMALL ? ATTR1_TINY : ATTR1_SMALL) | int(posX) & 0x01ff;
    self->sprite->attr2 = tileId | ATTR2_PRIORITY(2);
}

void destroyGem(GameObject* self) {
    if (self->data) {
        free(self->data);
    }
}

const GameObjectType gemType = {
    initializeGem,
    updateGem,
    drawGem,
    destroyGem
};

GameObject *spawnGem(Vector2 pos) {
    GameObject *newGem = newGameObject(&gemType);
    if (!newGem) return NULL;
    GemData *data = (GemData*)newGem->data;
    data->collider.pos.x = pos.x - data->collider.size.x/2;
    data->collider.pos.y = pos.y - data->collider.size.y/2;
    return newGem;
}

void randomizeGem(GameObject *self) {
    GemData *data = (GemData*)self->data;
    Vector2 vel;
    vel.x.value = randRange(-1<<16, 1<<16);
    vel.y.value = randRange(-1<<15, 1<<16);
    mgba_printf("Vel: %d,%d", vel.x, vel.y);
    vel = vel.normalized();
    vel = vel * fixed32(1.5);
    data->velocity = vel;
    data->rotationDirection = (rand() % 2) ? GEM_CCW : GEM_CW;
    data->type = (rand() % 4) ? GEM_SMALL : GEM_LARGE;
}

void collectGems(int gems) {
    gemsHeld += gems;
    totalGemsThisRun += gems;
}