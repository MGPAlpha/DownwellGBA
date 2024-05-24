#include "player.hpp"

#include <cstdlib>

#include "gamestate.hpp"
#include "cheats.hpp"
#include "engine/gameobject.hpp"
#include "bullet.hpp"
#include "enemy.hpp"
#include "engine/camera.hpp"
#include "sound.hpp"
#include "collision.hpp"
#include "engine/sprite.hpp"
#include <assets.hpp>
#include "engine/animation.hpp"

extern "C" {

#include "print.h"

#include "sfx/machinegunbulletc8a.h"
#include "sfx/playerdamagedd66.h"
#include "sfx/playerkilledc50.h"
#include "sfx/stompc96.h"

}

using namespace GBAEngine;


int jumpFrames[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,
    3,3,3,3,
    4
};

int Player::playerHealth = 4;
int Player::playerMaxHealth = 4;
int Player::playerMaxHealthProgress = 0;

void Player::checkForEnemyContact(Enemy *enemy) {
    if (this->state == PLAYER_DEAD) return;
    fixed32 oldVel = this->velocity.y;
    if (this->state == PLAYER_JUMPING || this->state == PLAYER_FALLING || this->state == PLAYER_HOP) {
        enemy->killEnemy();
        this->state = PLAYER_HOP;
        this->stateTime = 6;
        this->velocity.y = -3.4;
        this->runningJump = 1;
        this->ammo = this->charge;
        this->canFire = 0;
        playSoundBPriority(stompc96_data, stompc96_length, 0, 5);
    }
    if (this->iFrames < 1 && (this->state == PLAYER_HOP && oldVel < 0) || (this->state != PLAYER_FALLING && this->state != PLAYER_HOP)) {
        playerHealth--;
        playSoundBPriority(playerdamagedd66_data, playerdamagedd66_length, 0, 10);
        this->iFrames = 60;
    }

}

Player* Player::getSingleton() {
    return singleton;
}

Player::Player() {
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

Transform* Player::getTransform() { return this->transform; }

RectCollider* Player::getCollider() { return collider; }

Player* Player::singleton;

void Player::awake() {
    this->transform = this->getGameObject()->getComponent<Transform>();
    this->collider = this->getGameObject()->getComponent<RectCollider>();
    this->collider->onEnter += [this](Collider* c){
        if (Enemy* e = c->getComponent<Enemy>()) {
            this->checkForEnemyContact(e);
        }
    };
    this->spriteRenderer = this->getComponent<SpriteRenderer>();
    singleton = this;
}

void Player::update() {
    if (this->state != PLAYER_DEAD && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {
        if (BUTTON_HELD(BUTTON_LEFT)) {
            this->transform->position.x -= 2;
            this->dir = LEFT;
        }
        if (BUTTON_HELD(BUTTON_RIGHT)) {
            this->transform->position.x += 2;
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
    Collision walkCollision = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
    if (walkCollision.push.x) {
        this->transform->position.x += walkCollision.push.x;
    }

    if (this->state == PLAYER_IDLE || this->state == PLAYER_WALKING) {
        if (BUTTON_PRESSED(BUTTON_A) || BUTTON_PRESSED(BUTTON_B)) {
            this->runningJump = (this->state == PLAYER_WALKING) ? 1 : 0;
            this->state = PLAYER_JUMPING;
            this->stateTime = 0;
            this->velocity.y = -4.6;
            this->canFire = 0;
        } else {
            this->transform->position.y+= 1;
            Collision fallCollision = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
            if (fallCollision.push.y < 0) {
                this->transform->position.y += fallCollision.push.y;
            } else {
                this->state = PLAYER_JUMPING;
                this->runningJump = 0;
                this->velocity.y = 0;
                this->stateTime = 24;
            }
        }
    }

    if (this->state == PLAYER_JUMPING || this->state == PLAYER_HOP || this->state == PLAYER_DEAD) {
        if (this->state == PLAYER_JUMPING && !BUTTON_HELD(BUTTON_A) && !BUTTON_HELD(BUTTON_B) && this->stateTime < 12) {
            this->stateTime = 12;
            if (this->velocity.y < fixed32(-2.2)) this->velocity.y = -2.2;
        }
        this->velocity.y = this->velocity.y + fixed32(.2);
        this->velocity.y = min(this->velocity.y, 4);
        fixed32 jumpDisplacement = this->velocity.y;
        this->transform->position.y += jumpDisplacement;
        Collision yCollision = collideCollisionMap(this->collider->getRect(), activeCollisionMap, activeCollisionMapWidth, 20);
        if (yCollision.push.y) {
            this->transform->position.y += yCollision.push.y;
            this->velocity.y = 0;
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
            BulletPrefab* bullet = new BulletPrefab(this->transform->position);
            GameObject::loadGameObject(bullet);

            this->stateTime = 21;
            this->velocity.y = -.2;
            
            playSoundBPriority(machinegunbulletc8a_data, machinegunbulletc8a_length, 0, 0);
        }

        this->fireTime++;
    }

    // if (this->state != PLAYER_DEAD) GameObject::doForEachGameObject<Enemy>([this](Enemy* e) {
    //     this->checkForEnemyContact(e);
    //     return 0;
    // });

    if (this->state != PLAYER_DEAD && playerHealth <= 0) {
        this->state = PLAYER_DEAD;
        this->stateTime = 5;
        this->velocity.y = -3.6;
        stopSound();
        playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);
        REG_TM2CNT = 0;
    }

    if (this->state == PLAYER_DEAD) waitForVBlank();

    this->stateTime++;
    if (this->iFrames > 0) this->iFrames--;
}

void Player::draw() {
    return;
    int posY = this->transform->position.y - cameraPos.y - 4;
    int posX = (this->transform->position.x - cameraPos.x - 5);
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

PlayerPrefab::PlayerPrefab(Vector2 pos) {
    this->addComponent(new Transform(pos));
    RectCollider* col = new RectCollider(Vector2(6,11), RectCollider::CENTER);
    col->layer = L_0;
    col->mask = L_1 | L_2;
    this->addComponent(col);
    this->addComponent(new Player());
    this->addComponent(new SpriteRenderer(Assets::Animations::playerIdle.frames));
    this->addComponent(new SpriteAnimator(&Assets::Animations::playerRun));
}

PlayerPrefab::PlayerPrefab() : PlayerPrefab(Vector2(0,0)) {}