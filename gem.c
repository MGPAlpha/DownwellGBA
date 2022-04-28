#include "gem.h"

#include <stdlib.h>
#include "camera.h"
#include "collision.h"
#include "player.h"

int initializeGem(GameObject* this) {
    GemData *data = malloc(sizeof(GemData));
    if (!data) return 1;
    data->collider.pos.x = 0;
    data->collider.pos.y = 0;
    data->collider.size.x = 4<<8;
    data->collider.size.y = 4<<8;
    data->velocity.x = 0;
    data->velocity.y = 0;
    data->state = GEM_PHYSICS;
    data->stateTime = 0;
    data->type = GEM_SMALL;
    data->rotationDirection = GEM_CCW;
    this->data = data;

    return 0;
}

void updateGem(GameObject* this) {
    GemData *data = this->data;
    
    switch (data->state) {

        case GEM_PHYSICS:
            data->velocity.y += 1<<3;

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
                if (data->velocity.y >= 0) data->velocity.y = -1<<8;
                else data->velocity.y = -data->velocity.y;
            }

            if (playerSingleton) {
                PlayerData *playerData = playerSingleton->data;
                Vector2 playerCenter = V2_ADD(playerData->collider.pos, V2_DIV(playerData->collider.size,2));
                Vector2 gemCenter = V2_ADD(data->collider.pos, V2_DIV(data->collider.size,2));
                gemCenter.x >>= 8;
                gemCenter.y >>= 8;
                Vector2 playerDisp = V2_SUB(playerCenter, gemCenter);
                if (Vector2Length(playerDisp, 0) < 16) {
                    data->state = GEM_ATTRACT;
                    data->stateTime = 0;
                }

            }

            if (this->lifetime > 5*60) {
                destroyGameObject(this);
                return;
            }
            break;
        case GEM_ATTRACT:
            if (playerSingleton) {
                PlayerData *playerData = playerSingleton->data;
                Vector2 playerCenter = V2_ADD(playerData->collider.pos, V2_DIV(playerData->collider.size,2));
                Vector2 gemCenter = V2_ADD(data->collider.pos, V2_DIV(data->collider.size,2));
                playerCenter.x <<= 8;
                playerCenter.y <<= 8;
                Vector2 playerDisp = V2_SUB(playerCenter, gemCenter);
                int playerDistance = Vector2Length(playerDisp, 8);
                Vector2 accel = Vector2Normalize(playerDisp, 8);
                accel = V2_DIV(accel, 3);
                int accelRamp = data->stateTime;
                if (accelRamp) accel = V2_MUL(accel, accelRamp);
                // accel = V2_DIV(accel, (playerDistance * playerDistance) >> 16);
                // data->velocity.x += accel.x;
                // data->velocity.y += accel.y;

                data->collider.pos.x += data->velocity.x + accel.x;
                data->collider.pos.y += data->velocity.y + accel.y;

                Collision collisionWithPlayer = collideRects(playerData->resizedCollider, data->collider);
                if (collisionWithPlayer.collided) {
                    destroyGameObject(this);
                    return;
                }
            }

            data->stateTime++;
            break;
    }
}

void drawGem(GameObject* this) {
    GemData *data = this->data;
    int spOffset = data->type == GEM_SMALL ? 2 : 6;
    int posY = (data->collider.pos.y>>8) - cameraPos.y - spOffset;
    int posX = (data->collider.pos.x>>8) - cameraPos.x - spOffset;
    int aniFrame = this->lifetime/4;
    if (data->rotationDirection == GEM_CW) {
        aniFrame = -aniFrame;
    }
    aniFrame = mod(aniFrame, 4);
    int tileId = data->type == GEM_SMALL ? OFFSET(8,12,32) : OFFSET(0,12,32);
    tileId += aniFrame * (data->type == GEM_SMALL ? 1 : 2);
    if (this->lifetime > 60*4 && this->lifetime % 2 || posY < -16 || posY > 160 || posX < -16 || posX > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_SQUARE | posY & 0x00ff;
    this->sprite->attr1 = (data->type == GEM_SMALL ? ATTR1_TINY : ATTR1_SMALL) | posX & 0x01ff;
    this->sprite->attr2 = tileId | ATTR2_PRIORITY(2);
}

void destroyGem(GameObject* this) {
    if (this->data) {
        free(this->data);
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
    GemData *data = newGem->data;
    data->collider.pos.x = pos.x - data->collider.size.x/2;
    data->collider.pos.y = pos.y - data->collider.size.y/2;
    return newGem;
}

void randomizeGem(GameObject *this) {
    GemData *data = this->data;
    Vector2 vel;
    vel.x = randRange(-1<<8, 1<<8);
    vel.y = randRange(-1<<7, 1<<8);
    mgba_printf("Vel: %d,%d", vel.x, vel.y);
    vel = Vector2Normalize(vel, 8);
    vel = V2_MUL(vel, 3<<7);
    vel = V2_DIV(vel, 256);
    data->velocity = vel;
    data->rotationDirection = (rand() % 2) ? GEM_CCW : GEM_CW;
    data->type = (rand() % 4) ? GEM_SMALL : GEM_LARGE;
}