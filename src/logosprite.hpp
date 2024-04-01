#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "engine/gameobject.hpp"

// const extern GameObjectType logoSpriteType;

class LogoSprite : public GBAEngine::Component {
    public:
        LogoSprite(int index, GBAEngine::Vector2 pos);
        int animationStart = 0;
    protected:
        void draw() override;
    private:
        GBAEngine::Vector2 pos;
        int index;
};
#endif