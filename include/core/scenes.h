#ifndef SCENES_H
#define SCENES_H

// the gamescene enum
typedef enum { SCENE_MAIN, SCENE_GAME } GAMESCENE;

extern GAMESCENE currentScene;

// function prototypes

// scene swapping
void LoadScene(GAMESCENE scene);
void DrawScene(GAMESCENE scene);
void UpdateScene(GAMESCENE scene);
void UnloadScene(GAMESCENE scene);

// game scenes
void LoadGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);

void LoadMain(void);
void UpdateMain(void);
void DrawMain(void);
void UnloadMain(void);

#endif
