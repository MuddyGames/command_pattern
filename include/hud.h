#ifndef HUD_H
#define HUD_H

#include <raylib.h>

#include "command.h"

// HUD (Heads-Up Display)
// Shows which commands are active. It reacts to commands,
// but knows nothing about the player.
typedef struct
{
    Command lastCommand; // Command from the most recent frame
    int jumpCount;       // How many times JUMP has been pressed
} Hud;

// Set the HUD to its starting state
void InitHud(Hud *hud);

// React to this frame's command
void UpdateHud(Hud *hud, Command command);

// Draw the HUD on screen
void DrawHud(const Hud *hud);

#endif // HUD_H
