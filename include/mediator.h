#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <stdbool.h>

#include "command.h"
#include "player.h"
#include "hud.h"

// Define the Mediator structure
// Mediator knows about (player and HUD).
// Player and HUD don't know about each other.
typedef struct Mediator
{
    Player *player;
    Hud *hud;
} Mediator;

// Function to create a mediator instance
Mediator *CreateMediator(Player *player, Hud *hud);

// Handle Command
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime);

#endif
