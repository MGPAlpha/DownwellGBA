#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"
#include "engine/animation.hpp"

// const extern GameObjectType logoSpriteType;

class LogoSprite : public GBAEngine::Component {
    public:
        LogoSprite(GBAEngine::SpriteAnimator* anim1, GBAEngine::SpriteAnimator* anim2);
        void fadeIn();
        bool getStarted();
    protected:
        // void draw() override;
        void awake() override;
    private:
        GBAEngine::SpriteAnimator* anim1;
        GBAEngine::SpriteAnimator* anim2;
        GBAEngine::MovementAnimator* moveAnim;
        bool started = false;
};

class LogoSpritePrefab : public GBAEngine::GameObject {
    public:
        LogoSpritePrefab(GBAEngine::Vector2 pos);
};

#endif