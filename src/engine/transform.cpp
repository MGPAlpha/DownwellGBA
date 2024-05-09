#include "transform.hpp"

using namespace GBAEngine;

Transform::Transform() : Transform(Vector2(0,0)) {

}

Transform::Transform(Vector2 pos) {
    this->position = pos;
}

Transform::Transform(fixed32 x, fixed32 y) : Transform(Vector2(x,y)) {

}

