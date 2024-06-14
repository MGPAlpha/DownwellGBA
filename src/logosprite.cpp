#include "logosprite.hpp"

#include <cstdlib>

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"
#include "gamestate.hpp"
#include "engine/camera.hpp"
#include "engine/animation.hpp"
#include <assets.hpp>

extern "C" {

#include "print.h"

#include "art/logo.h"

}

using namespace GBAEngine;

LogoSprite::LogoSprite(SpriteAnimator* anim1, SpriteAnimator* anim2) {
    this->anim1 = anim1;
    this->anim2 = anim2;
}

void LogoSprite::fadeIn() {
    this->anim1->playAnimation(&Assets::Spritesheets::titleFade[0], false);
    this->anim2->playAnimation(&Assets::Spritesheets::titleFade[1], false);
    this->moveAnim->start();
    started = true;
}

bool LogoSprite::getStarted() {
    return started;
}

void LogoSprite::awake() {
    this->moveAnim = getComponent<MovementAnimator>();
}

LogoSpritePrefab::LogoSpritePrefab(Vector2 pos) {
    this->addComponent(new Transform(pos));
    SpriteAnimator* animator1 = new SpriteAnimator(nullptr, false);
    SpriteAnimator* animator2 = new SpriteAnimator(nullptr, false);
    SpriteRenderer* sprite1 = new SpriteRenderer();
    SpriteRenderer* sprite2 = new SpriteRenderer();
    sprite1->offset = Vector2(-32,0);
    sprite2->offset = Vector2(32,0);
    sprite1->renderPriority = 3;
    sprite2->renderPriority = 3;
    animator1->setRenderer(sprite1);
    animator2->setRenderer(sprite2);
    this->addComponent(sprite1);
    this->addComponent(sprite2);
    this->addComponent(animator1);
    this->addComponent(animator2);
    this->addComponent(new MovementAnimator(Vector2(0,-4), 80, EasingFunction::LINEAR));
    this->addComponent(new LogoSprite(animator1, animator2));
}
