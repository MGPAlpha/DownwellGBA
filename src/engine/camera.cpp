#include "camera.hpp"

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
    

    mgba_printf("current camera: %p", currentCamera);
    if (transitionCamera && transitionCamera != currentCamera) {
        if (activeTransition) {
            delete activeTransition;
        }
        activeTransition = new Transition(actualCameraPosition, transitionCamera, 20);
        currentCamera = transitionCamera;
        transitionCamera = nullptr;
    }
    if (activeTransition) {
        mgba_printf("using transition");
        actualCameraPosition = activeTransition->evaluate();
        mgba_printf("camera pos from eval: (%x, %x)", actualCameraPosition.x, actualCameraPosition.y);
        if (actualCameraPosition == currentCamera->smoothedPosition) {
            delete activeTransition;
            activeTransition = nullptr;
        }
    } else if (currentCamera) {
        actualCameraPosition = currentCamera->smoothedPosition;
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
    mgba_printf("eval Current time: %d", currTime);
    mgba_printf("eval Start time: %d", this->startTime);
    int elapsed = currTime - this->startTime;
    mgba_printf("eval Elapsed time: %d", elapsed);
    mgba_printf("Target cam: %x", targetCam);
    if (elapsed < 0) return startPos;
    if (elapsed > this->length) {
        return targetCam->smoothedPosition;
    }
    fixed32 t = fixed32(elapsed) / fixed32(this->length);
    mgba_printf("eval T: %x", t);
    return Vector2::lerp(startPos, targetCam->smoothedPosition, t);
}

Camera* CameraSystem::currentCamera;
Camera* CameraSystem::transitionCamera;
CameraSystem::Transition* CameraSystem::activeTransition;


Vector2 CameraSystem::getCameraPosition() {
    return CameraSystem::actualCameraPosition;
}


Vector2 cameraPos = {0,0};

