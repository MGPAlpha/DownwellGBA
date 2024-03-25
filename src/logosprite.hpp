#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "gameobject.hpp"

// const extern GameObjectType logoSpriteType;

class LogoSprite : public Component {
    public:
        LogoSprite(int index, Vector2 pos);
        int animationStart = 0;
    protected:
        void draw() override;
    private:
        Vector2 pos;
        int index;
};
#endif