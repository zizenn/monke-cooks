#ifndef LEVEL_H
#define LEVEL_H

typedef struct Level {
    int id;
    char* name;
    char* mapJsonPath;
    char* tileManifestJsonPath;
    char* textureJsonPath;

    void (*Load)(struct Level* self);
    void (*Update)(struct Level* self);
    void (*Draw)(struct Level* self);
    void (*Unload)(struct Level* self);
} Level;

extern Level* GameLevels;
extern int TotalGameLevels;
extern Level* currentLevel;

void LoadLevelManifest(const char* filename);
void SwitchToLevelByIndex(int index);
void SwitchToNextLevel(void);
void UnloadLevelManifest(void);

#endif
