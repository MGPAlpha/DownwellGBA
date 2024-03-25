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

int Player::playerHealth = 4;
int Player::playerMaxHealth = 4;
int Player::playerMaxHealthProgress = 0;

void checkForPlayerEnemyContact(GameObject *enemy, GameObject *player) {
    PlayerData *playerData = (PlayerData*)player->this;
    EnemyData *enemyData = (EnemyData*)enemy->this;
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

Player* Player::getSingleton() {
    return singleton;
}

Player::Player() {
    this->collider.pos.x = 0;
    this->collider.pos.y = 0;
    this->collider.size.x = 6;
    this->collider.size.y = 11;
    this->state = PLAYER_IDLE;
    this->stateTime = 0;
    this->dir = RIGHT;
    this->runningJump = 0;
    this->canFire = 0;
    this->fireTime = 0;
    this->charge = 8;
    this->ammo = 8;
    this->iFrames = 0;
}

void Player::awake() {
    singleton = this;
}

void Player::update() {
    if (this->state != PLAYER_DEAD && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {
        if (BUTTON_HELD(BUTTON_LEFT)) {
            this->collider.pos.x -= 2;
            this->dir = LEFT;
        }
        if (BUTTON_HELD(BUTTON_RIGHT)) {
            this->collider.pos.x += 2;
            this->dir = RIGHT;
        }
        if (this->state == PLAYER_IDLE){
            this->state = PLAYER_WALKING;
            this->stateTime = 0;
        }
    } else if (this->state == PLAYER_WALKING) {
        this->state = PLAYER_IDLE;
        this->stateTime = 0;
    }
    Collision walkCollision = collideCollisionMap(this->collider, activeCollisionMap, activeCollisionMapWidth, 20);
    if (walkCollision.push.x) {
        this->collider.pos.x += walkCollision.push.x;
    }

    if (this->state == PLAYER_IDLE || this->state == PLAYER_WALKING) {
        if (BUTTON_PRESSED(BUTTON_A) || BUTTON_PRESSED(BUTTON_B)) {
            this->runningJump = (this->state == PLAYER_WALKING) ? 1 : 0;
            this->state = PLAYER_JUMPING;
            this->stateTime = 0;
            this->canFire = 0;
        } else {
            this->collider.pos.y+= 1;
            Collision fallCollision = collideCollisionMap(this->collider, activeCollisionMap, activeCollisionMapWidth, 20);
            if (fallCollision.push.y < 0) {
                this->collider.pos.y += fallCollision.push.y;
            } else {
                this->state = PLAYER_JUMPING;
                this->runningJump = 0;
                this->stateTime = 24;
            }
        }
    }

    if (this->state == PLAYER_JUMPING || this->state == PLAYER_HOP || this->state == PLAYER_DEAD) {
        if (this->state == PLAYER_JUMPING && !BUTTON_HELD(BUTTON_A) && !BUTTON_HELD(BUTTON_B) && this->stateTime < 12) this->stateTime = 12;
        // playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);
        int jumpDiffIndex = this->stateTime;
        if (this->stateTime >= sizeof(jumpDisplacementFrames)/sizeof(int)) jumpDiffIndex = sizeof(jumpDisplacementFrames)/sizeof(int) - 1;
        int jumpDisplacement = jumpDisplacementFrames[jumpDiffIndex];
        this->collider.pos.y += jumpDisplacement;
        Collision yCollision = collideCollisionMap(this->collider, activeCollisionMap, activeCollisionMapWidth, 20);
        if (yCollision.push.y) {
            this->collider.pos.y += yCollision.push.y;
            if (yCollision.push.y < 0 && this->state != PLAYER_DEAD) {
                this->state = PLAYER_IDLE;
                this->ammo = this->charge;
            } else if (yCollision.push.y > 0) {
                this->stateTime = 24;
            }
        }
    }

    if (this->state == PLAYER_JUMPING || this->state == PLAYER_HOP) {
        if (!this->canFire && !BUTTON_HELD(BUTTON_A) && !BUTTON_HELD(BUTTON_B)) {
            this->canFire = 1;
            this->fireTime = 7;
        }

        if (this->canFire && (BUTTON_HELD(BUTTON_A) || BUTTON_HELD(BUTTON_B)) && this->fireTime >= 7 && (this->ammo > 0 || infiniteAmmoCheat)) {
            this->fireTime = 0;
            this->runningJump = 0;
            if (!infiniteAmmoCheat) this->ammo -= 1;
            GameObject *newBullet = newGameObject(&bulletType);
            this->stateTime = 21;
            if (newBullet->active) {
                BulletData *bulletData = (BulletData*)newBullet->this;
                bulletData->collider.pos.x = this->collider.pos.x;
                bulletData->collider.pos.y = this->collider.pos.y;
            }
            playSoundBPriority(machinegunbulletc8a_data, machinegunbulletc8a_length, 0, 0);
        }

        this->fireTime++;
    }

    Transform resizedCollider = this->collider;
    
    this->resizedCollider = resizedCollider;

    if (this->state != PLAYER_DEAD) doForEachGameObjectOfTypeWith(&enemyType, self, [](GameObject* g, void* a) {
        checkForPlayerEnemyContact(g, (GameObject*)a);
        return 0;
    });

    if (this->state != PLAYER_DEAD && playerHealth <= 0) {
        this->state = PLAYER_DEAD;
        this->stateTime = 5;
        stopSound();
        playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);
        REG_TM2CNT = 0;
    }

    if (this->state == PLAYER_DEAD) waitForVBlank();

    this->stateTime++;
    if (this->iFrames > 0) this->iFrames--;
}

void Player::draw() {
    int posY = this->collider.pos.y - cameraPos.y - 4;
    int posX = (this->collider.pos.x - cameraPos.x - 5);
    if ((this->iFrames > 0 && this->iFrames % 2) || posY < -16 || posY > 160 || posX < -16 || posX > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_SQUARE | posY & 0x00ff;
    this->sprite->attr1 = ATTR1_SMALL | posX & 0x01ff | this->dir << 12;
    if (this->state == PLAYER_IDLE) {
        this->sprite->attr2 = ATTR2_TILEID(vBlankCount / 16 % 4 * 2,0) | ATTR2_PRIORITY(2);
    } else if (this->state == PLAYER_WALKING) {
        this->sprite->attr2 = ATTR2_TILEID(vBlankCount / 4 % 8 * 2,2) | ATTR2_PRIORITY(2);
    } else if (this->state == PLAYER_JUMPING || this->state == PLAYER_HOP) {
        if (this->canFire && this->fireTime <= 5) {
            this->sprite->attr2 = ATTR2_TILEID(0,8) | ATTR2_PRIORITY(2);
        } else if (this->runningJump) {
            this->sprite->attr2 = ATTR2_TILEID(vBlankCount / 4 % 8 * 2,6) | ATTR2_PRIORITY(2);
        } else {
            int jumpAniFrame = this->stateTime;
            if (this->stateTime >= sizeof(jumpFrames)/sizeof(int)) jumpAniFrame = sizeof(jumpFrames)/sizeof(int) - 1;
            this->sprite->attr2 = ATTR2_TILEID(jumpFrames[jumpAniFrame] * 2,4) | ATTR2_PRIORITY(2);
        }
    } else if (this->state == PLAYER_DEAD) {
        this->sprite->attr2 = ATTR2_TILEID(this->stateTime < 60 ? 2 : 4,8) | ATTR2_PRIORITY(2);
    }
}

void Player::destroy() {
    if (singleton == this) {
        singleton = nullptr;
    }
}