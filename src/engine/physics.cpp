#include "physics.hpp"

using namespace GBAEngine;

void Collider::awake() {
    Physics::activeColliders.insert(this);
    this->transform = getComponent<Transform>();
}
void Collider::destroy() {
    Physics::activeColliders.erase(this);
}

std::set<Collider*, Collider::Comparator> Physics::activeColliders;


RectCollider::RectCollider(Vector2 size, Vector2 offset, PivotMode pivot) {
    this->size = size;
    this->offset = offset;
    this->pivot = pivot;
}
RectCollider::RectCollider(Vector2 size, Vector2 offset) : RectCollider(size, offset, PivotMode::CENTER) {

}
RectCollider::RectCollider(Vector2 size) : RectCollider(size, Vector2(0,0)) {

}
RectCollider::RectCollider(Vector2 size, PivotMode pivot) : RectCollider(size, Vector2(0,0), pivot) {

}
RectCollider::RectCollider(PivotMode pivot) : RectCollider(Vector2(0,0), pivot) {

}
RectCollider::RectCollider() : RectCollider(PivotMode::CENTER) {

}

Rect RectCollider::getRect() const {
    Vector2 pivotAdjustment;
    switch (pivot) {
        case PivotMode::CENTER:
            pivotAdjustment = this->size / -2;
            break;
        case PivotMode::TOP_LEFT:
            pivotAdjustment = Vector2(0);
            break;
        case PivotMode::TOP_RIGHT:
            pivotAdjustment = Vector2(-this->size.x, 0);
            break;
        case PivotMode::BOTTOM_LEFT:
            pivotAdjustment = Vector2(0, -this->size.y);
            break;
        case PivotMode::BOTTOM_RIGHT:
            pivotAdjustment = -this->size;
            break;
    }

    Rect result = Rect();
    result.pos = transform->position + pivotAdjustment + this->offset;
    result.size = this->size;
    return result;
}

