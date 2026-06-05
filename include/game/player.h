#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int gridX;
    int gridY;
    const char* sprite;
} Player;

extern Player player;

void InitPlayer(int startX, int startY);
void UpdatePlayer(void);
void DrawPlayer(void);

#endif
