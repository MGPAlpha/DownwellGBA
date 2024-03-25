#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "gameobject.hpp"

// const extern GameObjectType logoSpriteType;

class LogoSprite : Component {
    public:
        LogoSprite(int index, Vector2 pos);
    protected:
        void draw() override;
    private:
        Vector2 pos;
        int index;
        int animationStart = 0;
};
#endif