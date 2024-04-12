#include "camera.hpp"
#include "animation.hpp"

using namespace GBAEngine;

Camera::Camera(int priority, bool isEnabled) {
    this->priority = priority;
    this->isEnabled = isEnabled;
}

Camera::Camera(int priority) : Camera(priority, true) {}
Camera::Camera(bool isEnabled) : Camera(0, isEnabled) {}
Camera::Camera() : Camera(0, true) {}

void Camera::awake() {
    this->transform = getComponent<Transform>();
    if (this->isEnabled) {
        CameraSystem::registerCamera(this);
    }
    smoothedPosition = calculateTargetPos();

}

void Camera::lateUpdate() {
    Vector2 targetPos = calculateTargetPos();
    if (damping > 0) {
        smoothedPosition = smoothedPosition + (targetPos - smoothedPosition) / (damping + 1);
    } else {
        smoothedPosition = targetPos;
    }
}

void Camera::destroy() {
    if (this->isEnabled) {
        CameraSystem::deregisterCamera(this);
    }
}

void Camera::enable() {
    if (!this->isEnabled) {
        isEnabled = true;
        CameraSystem::registerCamera(this);
    }
}

void Camera::disable() {
    if (this->isEnabled) {
        isEnabled = false;
        CameraSystem::deregisterCamera(this);
    }
}

Vector2 Camera::calculateTargetPos() {
    if (lockX && lockY) return lockPos;
    Vector2 pos = transform->position + offset;
    if (lockX) pos.x = lockPos.x;
    else if (clampX) pos.x = clamp(clampMin.x, clampMax.x, pos.x);
    if (lockY) pos.y = lockPos.y;
    else if (clampY) pos.y = clamp(clampMin.y, clampMax.y, pos.y);
    return pos;
}


std::set<Camera*, Camera::Comparator> CameraSystem::activeCameras = std::set<Camera*, Camera::Comparator>();
Vector2 CameraSystem::actualCameraPosition;

void CameraSystem::registerCamera(Camera* cam) {
    activeCameras.insert(cam);
    if (*(activeCameras.begin()) == cam) {
        transitionCamera = cam;
    }
}
void CameraSystem::deregisterCamera(Camera* cam) {
    activeCameras.erase(cam);
    if (currentCamera == cam || transitionCamera == cam) {
        transitionCamera = *(activeCameras.begin());
        if (!transitionCamera) {
            delete activeTransition;
            activeTransition = nullptr;
        }
    }
}

void CameraSystem::update() {
    if (reset) {
        if (activeTransition) {
            delete activeTransition;
            activeTransition = nullptr;
        }
        reset = false;
        if (currentCamera || transitionCamera) {
            currentCamera = transitionCamera;
            transitionCamera = nullptr;
            actualCameraPosition = currentCamera->smoothedPosition;
        }
    } else {

        if (transitionCamera && transitionCamera != currentCamera) {
            if (activeTransition) {
                delete activeTransition;
            }
            activeTransition = new Transition(actualCameraPosition, transitionCamera, 40);
            currentCamera = transitionCamera;
            transitionCamera = nullptr;
        }
        if (activeTransition) {
            actualCameraPosition = activeTransition->evaluate();
            if (actualCameraPosition == currentCamera->smoothedPosition) {
                delete activeTransition;
                activeTransition = nullptr;
            }
        } else if (currentCamera) {
            actualCameraPosition = currentCamera->smoothedPosition;
        }
    }

    cameraPos = actualCameraPosition - Vector2(120,80);
}

CameraSystem::Transition::Transition(Vector2 startPos, Camera* target, int length) {
    this->startPos = startPos;
    this->targetCam = target;
    this->length = length;
    this->startTime = GameObject::getSceneLifetime();
}


Vector2 CameraSystem::Transition::evaluate() {
    int currTime = GameObject::getSceneLifetime();
    int elapsed = currTime - this->startTime;
    if (elapsed < 0) return startPos;
    if (elapsed > this->length) {
        return targetCam->smoothedPosition;
    }
    fixed32 t = fixed32(elapsed) / fixed32(this->length);
    t = EasingFunction::EASE_OUT(t);
    return Vector2::lerp(startPos, targetCam->smoothedPosition, t);
}

void CameraSystem::resetToPrimaryCam() {
    reset = true;
    actualCameraPosition = Vector2(0,0);
}

Camera* CameraSystem::currentCamera;
Camera* CameraSystem::transitionCamera;
CameraSystem::Transition* CameraSystem::activeTransition;
bool CameraSystem::reset = true;


Vector2 CameraSystem::getCameraPosition() {
    return CameraSystem::actualCameraPosition;
}


Vector2 cameraPos = {0,0};

