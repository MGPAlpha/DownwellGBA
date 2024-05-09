#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"

// const extern GameObjectType logoSpriteType;

class LogoSprite : public GBAEngine::Component {
    public:
        LogoSprite(int index);
        int animationStart = 0;
    protected:
        void draw() override;
        void awake() override;
    private:
        GBAEngine::Transform* transform;
        int index;
};
#endif