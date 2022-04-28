#include "gem.h"

#include <stdlib.h>
#include "camera.h"

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
    data->type = GEM_SMALL;
    data->rotationDirection = GEM_CCW;
    this->data = data;

    return 0;
}

void updateGem(GameObject* this) {
    GemData *data = this->data;
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
    if (posY < -16 || posY > 160 || posX < -16 || posX > 240) {
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
    vel.x = rand() % 1<<8;
    vel.y = rand() % 1<<8;
    vel = Vector2Normalize(vel, 8);
    vel = V2_MUL(vel, 1);
    data->velocity = vel;
    data->rotationDirection = (rand() % 2) ? GEM_CCW : GEM_CW;
    data->type = (rand() % 4) ? GEM_SMALL : GEM_LARGE;
}