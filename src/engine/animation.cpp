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
        mgba_printf("doing anim timer update");
        timer++;
        if (timer >= length) {
            transform->position = targetPos;
            started = false;
        } else {
            fixed32 t = fixed32(timer)/length;
            t = easing(t);
            transform->position = Vector2::lerp(startPos, targetPos, t);
            mgba_printf("start pos (%x, %x)", startPos.x, startPos.y);
            mgba_printf("target pos (%x, %x)", targetPos.x, targetPos.y);
            mgba_printf("cam pos (%x, %x)", transform->position.x, transform->position.y);
        }
    }
}