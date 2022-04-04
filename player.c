#include "player.h"

#include "print.h"
#include "collision.h"

#include "camera.h"
#include "gameobject.h"
#include "stdlib.h"
#include "gamestate.h"

int jumpFrames[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,
    3,3,3,3,
    4
};

int jumpDisplacementFrames[] = {
    -4,
    -5,
    -4,
    -3,
    -4,
    -3,
    -4,
    -3,
    -2,
    -3,
    -2,
    -3,
    -2,
    -1,
    -2,
    -1,
    -2,
    -1,
    0,
    -1,
    0,
    -1,
    0,
    1,
    0,
    1,
    0,
    1,
    2,
    1,
    2,
    1,
    2,
    3,
    2,
    3,
    2,
    3,
    4,
    3,
    4,
    3,
    4,
    5,
    4
};

int initializePlayer(GameObject* this) {
    PlayerData *data = malloc(sizeof(PlayerData));
    if (!data) return 1;
    data->collider.pos.x = 0;
    data->collider.pos.y = 0;
    data->collider.size.x = 6;
    data->collider.size.y = 8;
    data->state = PLAYER_IDLE;
    data->stateTime = 0;
    data->dir = RIGHT;
    data->runningJump = 0;
    this->data = data;

    return 0;
}

void updatePlayer(GameObject* this) {
    PlayerData *data = this->data;
    playerSingleton = this;
    if (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT)) {
        if (BUTTON_HELD(BUTTON_LEFT)) {
            data->collider.pos.x -= 2;
            data->dir = LEFT;
        }
        if (BUTTON_HELD(BUTTON_RIGHT)) {
            data->collider.pos.x += 2;
            data->dir = RIGHT;
        }
        if (data->state == PLAYER_IDLE){
            data->state = PLAYER_WALKING;
            data->stateTime = 0;
        }
    } else if (data->state == PLAYER_WALKING) {
        data->state = PLAYER_IDLE;
        data->stateTime = 0;
    }
    Collision walkCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 4);
    if (walkCollision.push.x) {
        data->collider.pos.x += walkCollision.push.x;
    }

    if (data->state == PLAYER_IDLE || data->state == PLAYER_WALKING) {
        if (BUTTON_PRESSED(BUTTON_A) || BUTTON_PRESSED(BUTTON_B)) {
            data->runningJump = (data->state == PLAYER_WALKING) ? 1 : 0;
            data->state = PLAYER_JUMPING;
            data->stateTime = 0;
        } else {
            data->collider.pos.y++;
            Collision fallCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 4);
            if (fallCollision.push.y < 0) {
                data->collider.pos.y += fallCollision.push.y;
            } else {
                data->state = PLAYER_JUMPING;
                data->runningJump = 0;
                data->stateTime = 24;
            }
        }
    }

    if (data->state == PLAYER_JUMPING) {
        if (!BUTTON_HELD(BUTTON_A) && !BUTTON_HELD(BUTTON_B) && data->stateTime < 10) data->stateTime = 10;
        int jumpDiffIndex = data->stateTime;
        if (data->stateTime >= sizeof(jumpDisplacementFrames)/sizeof(int)) jumpDiffIndex = sizeof(jumpDisplacementFrames)/sizeof(int) - 1;
        int jumpDisplacement = jumpDisplacementFrames[jumpDiffIndex];
        data->collider.pos.y += jumpDisplacement;
        Collision yCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 4);
        if (yCollision.push.y) {
            data->collider.pos.y += yCollision.push.y;
            if (yCollision.push.y < 0) data->state = PLAYER_IDLE;
        }
    }

    data->stateTime++;
}

void drawPlayer(GameObject* this) {
    PlayerData *data = this->data;
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_SQUARE | (data->collider.pos.y - cameraPos.y - 7) & 0x00ff;
    this->sprite->attr1 = ATTR1_SMALL | (data->collider.pos.x - cameraPos.x - 5) & 0x01ff | data->dir << 12;
    if (data->state == PLAYER_IDLE) {
        this->sprite->attr2 = ATTR2_TILEID(vBlankCount / 16 % 4 * 2,0) | ATTR2_PRIORITY(2);
    } else if (data->state == PLAYER_WALKING) {
        this->sprite->attr2 = ATTR2_TILEID(vBlankCount / 4 % 8 * 2,2) | ATTR2_PRIORITY(2);
    } else if (data->state == PLAYER_JUMPING) {
        if (data->runningJump) {
            this->sprite->attr2 = ATTR2_TILEID(vBlankCount / 4 % 8 * 2,6) | ATTR2_PRIORITY(2);
        } else {
            int jumpAniFrame = data->stateTime;
            if (data->stateTime >= sizeof(jumpFrames)/sizeof(int)) jumpAniFrame = sizeof(jumpFrames)/sizeof(int) - 1;
            this->sprite->attr2 = ATTR2_TILEID(jumpFrames[jumpAniFrame] * 2,4) | ATTR2_PRIORITY(2);
        }
    }
}

void destroyPlayer(GameObject* this) {
    if (this->data) {
        free(this->data);
    }
    playerSingleton = NULL;
}

const GameObjectType playerType = {
    initializePlayer,
    updatePlayer,
    drawPlayer,
    destroyPlayer
};

GameObject *playerSingleton = NULL;
