#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "constants.h"

// Player Structure Data
typedef struct
{
    float x;
    float y;
    int r;
    float speed;
    int health;
    float breathTimer;
    Color color;
} Player;

// Player Behaviours
void Idle(Player *player, float deltaTime);
void MoveUp(Player *player, float deltaTime);
void MoveDown(Player *player, float deltaTime);
void MoveLeft(Player *player, float deltaTime);
void MoveRight(Player *player, float deltaTime);

void Jump(Player *player, float deltaTime);
void Fire(Player *player, float deltaTime);

void JumpFire(Player *player, float deltaTime);
void MoveUpFire(Player *player, float deltaTime);

void ClampPlayerOnScreen(Player *player); // Player stays within screen bounds

#endif
