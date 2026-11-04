#include <stdbool.h>
#include <stdlib.h>

#include "mediator.h"
#include "player.h"
#include "hud.h"
#include "command.h"

// For a good explanation on Mediator Pattern see
// https://www.geeksforgeeks.org/mediator-design-pattern/
// https://refactoring.guru/design-patterns/mediator

// Creates and initializes a new mediator instance
Mediator *CreateMediator(Player *player, Hud *hud)
{
    Mediator *mediator = (Mediator *)malloc(sizeof(Mediator));
    mediator->player = player;
    mediator->hud = hud;
    return mediator;
}

// Function to handle command and what needs to be done with it. The mediator is the only place that knows about both the player and the HUD.
void MediatorHandleCommand(Mediator *mediator, Command command, float deltaTime)
{
    // One command, two collaborators. Neither knows about the other:
    // The mediator is the only place that knows about both.

    // 1. Player acts on the command (via the Command System)
    ExecuteCommand(command, mediator->player, deltaTime);

    // 2. HUD shows the command
    UpdateHud(mediator->hud, command);
}
