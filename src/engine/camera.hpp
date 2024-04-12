#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "gbamath.hpp"
#include "gameobject.hpp"
#include "transform.hpp"
#include "animation.hpp"

namespace GBAEngine {

    class Camera;
    class CameraSystem;

    class Camera : public GBAEngine::Component {
        friend class CameraSystem;
        public:
            Camera(int priority, bool isEnabled);
            Camera(bool isEnabled);
            Camera(int priority);
            Camera();

            void enable();
            void disable();

            Vector2 offset = Vector2(0);
            bool lockX = false, lockY = false;
            Vector2 lockPos = Vector2(0);
            bool clampX = false, clampY = false;
            Vector2 clampMin = Vector2(0), clampMax = Vector2(0);
            fixed32 damping = 0;

            struct Comparator {
                bool operator()(const Camera* lhs, const Camera* rhs) const {
                    return lhs->priority > rhs->priority;
                }
            };
        protected:
            void awake() override;
            void lateUpdate() override;
            void destroy() override;
        private:
            Transform* transform;

            Vector2 calculateTargetPos();

            bool isEnabled = true;
            int priority = 0;

            Vector2 smoothedPosition = Vector2(0);
    };

    class CameraSystem {
        friend class Camera;
        public:
            static Vector2 getCameraPosition();
            static void update();
        private:
            
            static Vector2 actualCameraPosition;

            static std::set<Camera*, Camera::Comparator> activeCameras;

            static void registerCamera(Camera*);
            static void deregisterCamera(Camera*);

            static Camera* currentCamera;
            static Camera* transitionCamera;

            struct Transition {
                int startTime;
                Vector2 startPos;
                Camera* targetCam;
                int length;

                Transition(Vector2 startPos, Camera* target, int length);
                Vector2 evaluate();
            };
            static Transition* activeTransition;
    };

};

extern GBAEngine::Vector2 cameraPos;

#endif