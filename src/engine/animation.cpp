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