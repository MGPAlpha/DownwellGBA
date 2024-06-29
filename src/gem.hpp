#ifndef GEM_HPP
#define GEM_HPP

#include "engine/gameobject.hpp"
#include "engine/physics.hpp"
#include "engine/animation.hpp"

enum GEMSTATE {GEM_PHYSICS, GEM_ATTRACT};
enum GEMTYPE {GEM_SMALL, GEM_LARGE};
enum GEMROTATION {GEM_CW, GEM_CCW};

class Gem : public GBAEngine::Component {
    
    public:
        Gem();

    protected:
        void awake() override;
        void update() override;
        void draw() override;

    private:
        GBAEngine::Transform* transform;
        GBAEngine::RectCollider* collider;
        GBAEngine::SpriteAnimator* animator;

        GBAEngine::Vector2 velocity;
        enum GEMSTATE state;
        int stateTime;
        enum GEMTYPE type;
        enum GEMROTATION rotationDirection;

        void randomizeGem();

};

extern int gemsHeld;
extern int totalGemsThisRun;
extern int lifetimeGems;

void initGemData(void);

class GemPrefab : public GBAEngine::GameObject {
    public:
        GemPrefab(GBAEngine::Vector2);
        GemPrefab();
};

void collectGems(int gems);



#endif