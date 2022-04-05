#ifndef GAMESTATE_H
#define GAMESTATE_H

enum GAMESTATE {GAME_SURFACE, GAME_PAUSE, GAME_WIN};
extern enum GAMESTATE gameState;

extern int stateTime;
extern int vBlankCount;

void initSurface(void);

void updateSurface(void);

void pauseFromSurface(void);

void updatePause(void);

void initWin(void);

void updateWin(void);

#endif