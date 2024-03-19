#ifndef GAMESTATE_H
#define GAMESTATE_H

enum GAMESTATE {GAME_SURFACE, GAME_PAUSE, GAME_WIN, GAME_LOSE, GAME_PLAY, GAME_INSTRUCTIONS};
extern enum GAMESTATE gameState;
extern enum GAMESTATE unpauseState;

extern int stateTime;
extern int vBlankCount;

void initSurface(void);

void updateSurface(void);

void pauseFromSurface(void);

void unpause(void);

void updatePause(void);

void initWin(void);

void updateWin(void);

void initGame(void);

void updateGame(void);

void initLose(void);

void updateLose(void);

void initInstructions(void);

void updateInstructions(void);

#endif