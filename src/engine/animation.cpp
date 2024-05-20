#include "animation.hpp"

using namespace GBAEngine;

EasingFunction::EasingFunction(std::function<fixed32(fixed32)> f) {
    this->func = f;
}

fixed32 EasingFunction::operator()(fixed32 t) const {
    return this->func(t);
}

const EasingFunction EasingFunction::LINEAR = EasingFunction([](fixed32 t){
    return t;
});
const EasingFunction EasingFunction::EASE_IN = EasingFunction([](fixed32 t){
    return t*t;
});
const EasingFunction EasingFunction::EASE_OUT = EasingFunction([](fixed32 t){
    return -t*t+2*t;
});

MovementAnimator::MovementAnimator(Vector2 movement, int length, EasingFunction easing) {
    this->movement = movement;
    this->length = length;
    this->easing = easing;
}

void MovementAnimator::start() {
    this->startPos = transform->position;
    this->targetPos = this->startPos + this->movement;
    this->timer = 0;
    this->started = true;
}

void MovementAnimator::awake() {
    this->transform = getComponent<Transform>();
}

void MovementAnimator::update() {
    if (started) {
        timer++;
        if (timer >= length) {
            transform->position = targetPos;
            started = false;
        } else {
            fixed32 t = fixed32(timer)/length;
            t = easing(t);
            transform->position = Vector2::lerp(startPos, targetPos, t);
        }
    }
}

SpriteAnimator::SpriteAnimator(const SpriteAnimation* anim) {
    this->currentAnimation = anim;
}

SpriteAnimator::SpriteAnimator() : SpriteAnimator(nullptr) {}

void SpriteAnimator::awake() {
    this->spriteRenderer = getComponent<SpriteRenderer>();
    if (this->currentAnimation) {
        loadAnimation(this->currentAnimation);
    }
}

void SpriteAnimator::loadAnimation(const SpriteAnimation* anim) {
    unloadAnimation();
    this->currentAnimation = anim;
    if (this->currentAnimation) {
        for (int i = 0; i < this->currentAnimation->frameCount; i++) {
            auto loadedSprite = SpriteAllocator::checkoutSprite(this->currentAnimation->frames + i);
            if (loadedSprite) this->loadedSprites.insert(this->currentAnimation->frames + i);
        }
        this->totalAnimLength = anim->frameCount * anim->frameLength;
        this->animTimer = 0;
        this->currentFrame = -1;
    }
}

void SpriteAnimator::unloadAnimation() {
    this->currentAnimation = nullptr;
    for (auto sp : loadedSprites) {
        SpriteAllocator::returnSprite(sp);
    }
}

void SpriteAnimator::destroy() {
    unloadAnimation();
}

void SpriteAnimator::lateUpdate() {
    int newFrameIndex = this->animTimer / this->currentAnimation->frameLength;
    if (newFrameIndex != this->currentFrame) {
        if (this->spriteRenderer) {
            this->currentFrame = newFrameIndex;
            this->spriteRenderer->setSprite(this->currentAnimation->frames + this->currentFrame);
        }
    }
    this->animTimer++;
    if (this->animTimer >= this->totalAnimLength) this->animTimer = 0;
}