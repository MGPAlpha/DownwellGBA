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

SpriteAnimator::SpriteAnimator(const SpriteAnimation* anim, bool preloadFrames) {
    this->currentAnimation = anim;
    this->preloadFrames = preloadFrames;
}

SpriteAnimator::SpriteAnimator(const SpriteAnimation* anim) : SpriteAnimator(anim, true) {}

SpriteAnimator::SpriteAnimator() : SpriteAnimator(nullptr) {}

void SpriteAnimator::awake() {
    
    if (!this->spriteRenderer) {
        this->spriteRenderer = getComponent<SpriteRenderer>();
    }
    if (this->currentAnimation) {
        loadAnimation(this->currentAnimation);
    }
}

void SpriteAnimator::setRenderer(SpriteRenderer* r) {
    this->spriteRenderer = r;
}


void SpriteAnimator::loadAnimation(const SpriteAnimation* anim) {
    unloadAnimation();
    this->currentAnimation = anim;
    if (this->currentAnimation) {
        if (preloadFrames) {
            for (int i = 0; i < this->currentAnimation->frameCount; i++) {
                auto loadedSprite = SpriteAllocator::checkoutSprite(this->currentAnimation->frames + i);
                mgba_printf("loaded sprite %p at tile %d", this->currentAnimation->frames + i, loadedSprite->getIndex());
                if (loadedSprite) this->loadedSprites.insert(this->currentAnimation->frames + i);
            }
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
    loadedSprites.clear();
}

void SpriteAnimator::playAnimation(const SpriteAnimation* anim, int frame, bool loop) {
    if (this->currentAnimation != anim) {
        loadAnimation(anim);
    }
    if (anim) this->animTimer = frame * anim->frameLength;
    else this->animTimer = 0;
    this->loop = loop;
}
void SpriteAnimator::playAnimation(const SpriteAnimation* anim, int frame) {
    this->playAnimation(anim, frame, true);
}
void SpriteAnimator::playAnimation(const SpriteAnimation* anim, bool loop) {
    this->playAnimation(anim, 0, loop);
}
void SpriteAnimator::playAnimation(const SpriteAnimation* anim) {
    this->playAnimation(anim, true);
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
    if (this->animTimer >= this->totalAnimLength) {
        if (this->loop) this->animTimer = 0;
        else this->animTimer = this->totalAnimLength - 1;
    }
}