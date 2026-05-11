#ifndef TARGETGAME_H
#define TARGETGAME_H

#include "core/enums.h"

extern TARGET_GAME_RESULT targetGameResult;

void InitTargetMinigame(void);
void UpdateTargetMinigame(void);
void DrawTargetMinigame(void);
void UnloadTargetMinigame(void);

#endif
