#include "physics.hpp"

using namespace GBAEngine;

void Collider::awake() {
    Physics::activeColliders.insert(this);
    this->transform = getComponent<Transform>();
}
void Collider::destroy() {
    Physics::activeColliders.erase(this);
}

bool Collider::needsIntersectionCheck() {
    return onEnter.size() > 0 || onStay.size() > 0 || onExit.size() > 0;
}

void Collider::registerIntersection(Collider* c) {
    this->onStay(c);
    this->currentFrameIntersections.insert(c);
    if (this->lastFrameIntersections.count(c) == 0) {
        this->onEnter(c);
    }
}
void Collider::registerNonintersection(Collider* c) {
    if (this->lastFrameIntersections.count(c) > 0) {
        this->onExit(c);
    }
}


ColliderFunctionTable<bool> Physics::intersectionHandlers;
ColliderFunctionTable<Collision, Vector2> Physics::collisionHandlers;

std::set<Collider*, Collider::Comparator> Physics::activeColliders;

void Physics::updateIntersections() {
    auto iterA = activeColliders.begin();
    auto colEnd = activeColliders.end();
    while (iterA != colEnd) {
        auto iterB = iterA;
        iterB++;
        Collider* colA = *iterA;
        while (iterB != colEnd) {
            Collider* colB = *iterB;
            bool aChecksIntersection = colA->needsIntersectionCheck();
            bool bChecksIntersection = colB->needsIntersectionCheck();
            bool aSeesB = colB->layer & colA->mask;
            bool bSeesA = colA->layer & colB->mask;
            bool aShouldCheck = aSeesB && aChecksIntersection;
            bool bShouldCheck = bSeesA && bChecksIntersection;
            if (aShouldCheck || bShouldCheck) {
                bool intersected = Physics::intersectionHandlers.check(colA, colB);
                if (intersected) {
                    if (aShouldCheck) colA->registerIntersection(colB);
                    if (bShouldCheck) colB->registerIntersection(colA);
                } else {
                    if (aShouldCheck) colA->registerNonintersection(colB);
                    if (bShouldCheck) colB->registerNonintersection(colA);
                }
            }
            iterB++;
        }
        iterA++;
    }

    for (Collider* c : activeColliders) {
        c->lastFrameIntersections = c->currentFrameIntersections;
        c->currentFrameIntersections.clear();
    }

}

RectCollider::constructor RectCollider::cons;

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

bool RectCollider::collideRect(RectCollider* a, RectCollider* b) {
    Rect rectA = a->getRect();
    Rect rectB = b->getRect();

    Collision col = collideRects(rectA, rectB);
    return col.collided;
}


