#ifndef GAMESTATE_H
#define GAMESTATE_H

enum GAMESTATE {GAME_SURFACE};
extern enum GAMESTATE gameState;

extern int stateTime;
extern int vBlankCount;

void initSurface(void);

void updateSurface(void);

#endif