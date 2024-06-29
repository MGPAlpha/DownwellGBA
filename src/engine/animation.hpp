#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <functional>
#include <unordered_set>
#include "sprite.hpp"
#include "gbamath.hpp"
#include "transform.hpp"

namespace GBAEngine {
    class EasingFunction {
        public:
            EasingFunction(std::function<fixed32(fixed32)> f);

            // Call the easing function, remaps t to an output 0->0, 1->1, (0,1)->(0,1), (-inf,0)U(1,inf) -> undefined
            fixed32 operator()(fixed32 t) const;
            static const EasingFunction EASE_OUT;
            static const EasingFunction EASE_IN;
            static const EasingFunction LINEAR;
        private:
            std::function<fixed32(fixed32)> func;
    };

    class MovementAnimator : public Component {
        public:
            MovementAnimator(Vector2 movement, int length, EasingFunction easing);
            MovementAnimator(Vector2 movement, int length);
            void start();
            bool startOnAwake = false;
        protected:
            void update() override;
            void awake() override;
        private:
            Transform* transform;
            EasingFunction easing = EasingFunction::LINEAR;
            Vector2 movement;
            Vector2 targetPos;
            bool started = false;
            Vector2 startPos;
            int length;
            int timer = 0;
    };

    class SpriteAnimator : public Component {
        public:
            SpriteAnimator(const SpriteAnimation* anim, bool preloadFrames);
            SpriteAnimator(const SpriteAnimation* anim);
            SpriteAnimator();

            void playAnimation(const SpriteAnimation*);
            void playAnimation(const SpriteAnimation*, int frame);
            void playAnimation(const SpriteAnimation*, bool loop);
            void playAnimation(const SpriteAnimation*, int frame, bool loop);

            void setRenderer(SpriteRenderer* r);

            bool loop = true;
            bool reversed = false;
        protected:
            void awake() override;
            void lateUpdate() override;
            void destroy() override;
        private:
            SpriteRenderer* spriteRenderer = nullptr;
            const SpriteAnimation* currentAnimation = nullptr;
            std::unordered_set<const Sprite*> loadedSprites;
            void loadAnimation(const SpriteAnimation* anim);
            void unloadAnimation();
            int animTimer = 0;
            int totalAnimLength = 0;
            int currentFrame = 0;
            bool preloadFrames = true;
    };
}

#endif