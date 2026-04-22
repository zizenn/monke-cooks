#ifndef TARGETGAME_H
#define TARGETGAME_H

typedef enum {
    TARGET_GAME_RUNNING,
    TARGET_GAME_WIN,
    TARGET_GAME_LOSE

} TargetGameResult;

extern TargetGameResult targetGameResult;

#endif