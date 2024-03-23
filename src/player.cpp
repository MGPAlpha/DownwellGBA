#include "player.hpp"

#include <cstdlib>

#include "gamestate.hpp"
#include "cheats.hpp"
#include "gameobject.hpp"
#include "bullet.hpp"
#include "enemy.hpp"
#include "camera.hpp"
#include "sound.hpp"
#include "collision.hpp"

extern "C" {

#include "print.h"

#include "sfx/machinegunbulletc8a.h"
#include "sfx/playerdamagedd66.h"
#include "sfx/playerkilledc50.h"
#include "sfx/stompc96.h"

}


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

int playerHealth = 4;
int playerMaxHealth = 4;
int playerMaxHealthProgress = 0;

void checkForPlayerEnemyContact(GameObject *enemy, GameObject *player) {
    PlayerData *playerData = (PlayerData*)player->data;
    EnemyData *enemyData = (EnemyData*)enemy->data;
    Collision col = collideRects(playerData->resizedCollider, enemyData->collider);
    if (col.collided) {
        if (col.push.y < 0/* && col.push.y > -5<<8*/) {
            killEnemy(enemy);
            playerData->state = PLAYER_HOP;
            playerData->stateTime = 6;
            playerData->runningJump = 1;
            playerData->ammo = playerData->charge;
            playerData->canFire = 0;
            playSoundBPriority(stompc96_data, stompc96_length, 0, 5);
        } else if (playerData->iFrames < 1) {
            playerHealth--;
            playSoundBPriority(playerdamagedd66_data, playerdamagedd66_length, 0, 10);
            playerData->iFrames = 60;
        }
    }
}

int initializePlayer(GameObject* self) {
    PlayerData *data = (PlayerData*)malloc(sizeof(PlayerData));
    if (!data) return 1;
    data->collider.pos.x = 0;
    data->collider.pos.y = 0;
    data->collider.size.x = 6;
    data->collider.size.y = 11;
    data->state = PLAYER_IDLE;
    data->stateTime = 0;
    data->dir = RIGHT;
    data->runningJump = 0;
    data->canFire = 0;
    data->fireTime = 0;
    data->charge = 8;
    data->ammo = 8;
    data->iFrames = 0;
    self->data = data;

    return 0;
}

void updatePlayer(GameObject* self) {
    PlayerData *data = (PlayerData*)self->data;
    playerSingleton = self;
    if (data->state != PLAYER_DEAD && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {
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
    Collision walkCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 20);
    if (walkCollision.push.x) {
        data->collider.pos.x += walkCollision.push.x;
    }

    if (data->state == PLAYER_IDLE || data->state == PLAYER_WALKING) {
        if (BUTTON_PRESSED(BUTTON_A) || BUTTON_PRESSED(BUTTON_B)) {
            data->runningJump = (data->state == PLAYER_WALKING) ? 1 : 0;
            data->state = PLAYER_JUMPING;
            data->stateTime = 0;
            data->canFire = 0;
        } else {
            data->collider.pos.y+= 1;
            mgba_printf("Player Y before collision: %x", data->collider.pos.y);
            Collision fallCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 20);
            if (fallCollision.push.y < 0) {
                data->collider.pos.y += fallCollision.push.y;
            } else {
                data->state = PLAYER_JUMPING;
                data->runningJump = 0;
                data->stateTime = 24;
            }
        }
    }

    if (data->state == PLAYER_JUMPING || data->state == PLAYER_HOP || data->state == PLAYER_DEAD) {
        if (data->state == PLAYER_JUMPING && !BUTTON_HELD(BUTTON_A) && !BUTTON_HELD(BUTTON_B) && data->stateTime < 12) data->stateTime = 12;
        // playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);
        int jumpDiffIndex = data->stateTime;
        if (data->stateTime >= sizeof(jumpDisplacementFrames)/sizeof(int)) jumpDiffIndex = sizeof(jumpDisplacementFrames)/sizeof(int) - 1;
        int jumpDisplacement = jumpDisplacementFrames[jumpDiffIndex];
        data->collider.pos.y += jumpDisplacement;
        Collision yCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 4);
        if (yCollision.push.y) {
            data->collider.pos.y += yCollision.push.y;
            if (yCollision.push.y < 0 && data->state != PLAYER_DEAD) {
                data->state = PLAYER_IDLE;
                data->ammo = data->charge;
            } else if (yCollision.push.y > 0) {
                data->stateTime = 24;
            }
        }
    }

    if (data->state == PLAYER_JUMPING || data->state == PLAYER_HOP) {
        if (!data->canFire && !BUTTON_HELD(BUTTON_A) && !BUTTON_HELD(BUTTON_B)) {
            data->canFire = 1;
            data->fireTime = 7;
        }

        if (data->canFire && (BUTTON_HELD(BUTTON_A) || BUTTON_HELD(BUTTON_B)) && data->fireTime >= 7 && (data->ammo > 0 || infiniteAmmoCheat)) {
            data->fireTime = 0;
            data->runningJump = 0;
            if (!infiniteAmmoCheat) data->ammo -= 1;
            GameObject *newBullet = newGameObject(&bulletType);
            data->stateTime = 21;
            if (newBullet->active) {
                BulletData *bulletData = (BulletData*)newBullet->data;
                bulletData->collider.pos.x = data->collider.pos.x;
                bulletData->collider.pos.y = data->collider.pos.y;
            }
            playSoundBPriority(machinegunbulletc8a_data, machinegunbulletc8a_length, 0, 0);
        }

        data->fireTime++;
    }

    Transform resizedCollider = data->collider;
    
    data->resizedCollider = resizedCollider;

    if (data->state != PLAYER_DEAD) doForEachGameObjectOfTypeWith(&enemyType, self, [](GameObject* g, void* a) {
        checkForPlayerEnemyContact(g, (GameObject*)a);
        return 0;
    });

    if (data->state != PLAYER_DEAD && playerHealth <= 0) {
        data->state = PLAYER_DEAD;
        data->stateTime = 5;
        stopSound();
        playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);
        REG_TM2CNT = 0;
    }

    if (data->state == PLAYER_DEAD) waitForVBlank();

    data->stateTime++;
    if (data->iFrames > 0) data->iFrames--;
}

void drawPlayer(GameObject* self) {
    PlayerData *data = (PlayerData*)self->data;
    int posY = data->collider.pos.y - cameraPos.y - 4;
    int posX = (data->collider.pos.x - cameraPos.x - 5);
    if ((data->iFrames > 0 && data->iFrames % 2) || posY < -16 || posY > 160 || posX < -16 || posX > 240) {
        self->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    self->sprite->attr0 = ATTR0_REGULAR | ATTR0_SQUARE | posY & 0x00ff;
    self->sprite->attr1 = ATTR1_SMALL | posX & 0x01ff | data->dir << 12;
    if (data->state == PLAYER_IDLE) {
        self->sprite->attr2 = ATTR2_TILEID(vBlankCount / 16 % 4 * 2,0) | ATTR2_PRIORITY(2);
    } else if (data->state == PLAYER_WALKING) {
        self->sprite->attr2 = ATTR2_TILEID(vBlankCount / 4 % 8 * 2,2) | ATTR2_PRIORITY(2);
    } else if (data->state == PLAYER_JUMPING || data->state == PLAYER_HOP) {
        if (data->canFire && data->fireTime <= 5) {
            self->sprite->attr2 = ATTR2_TILEID(0,8) | ATTR2_PRIORITY(2);
        } else if (data->runningJump) {
            self->sprite->attr2 = ATTR2_TILEID(vBlankCount / 4 % 8 * 2,6) | ATTR2_PRIORITY(2);
        } else {
            int jumpAniFrame = data->stateTime;
            if (data->stateTime >= sizeof(jumpFrames)/sizeof(int)) jumpAniFrame = sizeof(jumpFrames)/sizeof(int) - 1;
            self->sprite->attr2 = ATTR2_TILEID(jumpFrames[jumpAniFrame] * 2,4) | ATTR2_PRIORITY(2);
        }
    } else if (data->state == PLAYER_DEAD) {
        self->sprite->attr2 = ATTR2_TILEID(data->stateTime < 60 ? 2 : 4,8) | ATTR2_PRIORITY(2);
    }
}

void destroyPlayer(GameObject* self) {
    if (self->data) {
        free(self->data);
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