#ifndef GEM_HPP
#define GEM_HPP

#include "gameobject.hpp"

enum GEMSTATE {GEM_PHYSICS, GEM_ATTRACT};
enum GEMTYPE {GEM_SMALL, GEM_LARGE};
enum GEMROTATION {GEM_CW, GEM_CCW};

class Gem : public Component {
    
    public:
        Gem(Vector2 pos);

    protected:
        void update() override;
        void draw() override;

    private:
        Rect collider;
        Vector2 velocity;
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

void collectGems(int gems);



#endif