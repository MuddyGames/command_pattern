#include <raylib.h>

#include <stdio.h>
#include <math.h>

#include "player.h"

#define JUMP_HEIGHT 150.0f		// pixels, once per jump
#define RECOIL_SPEED 180.0f		// pixels per second while firing

// Keep player in screen bounds
void ClampPlayerOnScreen(Player *player)
{
	if (player->x < player->r)
		player->x = player->r;
	if (player->x > SCREEN_WIDTH - player->r)
		player->x = SCREEN_WIDTH - player->r;
	if (player->y < player->r)
		player->y = player->r;
	if (player->y > SCREEN_HEIGHT - player->r)
		player->y = SCREEN_HEIGHT - player->r;
}

// Player Idle Behaviour
void Idle(Player *player, float deltaTime)
{
	// Simple breathing effect by changing radius over time
	// Idle animation
	player->breathTimer += deltaTime;
	player->color = GREEN; // Change color to indicate idle or default color
	float wave = sinf(player->breathTimer * 3.0f);
	float scale = 1.0f + (wave * 0.05f);

	player->r = (int)(DEFAULT_PLAYER_RADIUS * scale);
}

// Move player up
void MoveUp(Player *player, float deltaTime)
{
	// Simple move up
	player->y -= player->speed * deltaTime;
	player->color = GREEN; // Change color to default color
	ClampPlayerOnScreen(player);
}

// Move up and Fire
void MoveUpFire(Player *player, float deltaTime)
{
	// Move up and fire
	player->y -= player->speed * deltaTime;
	player->x -= RECOIL_SPEED * deltaTime; // Recoil effect
	player->color = RED; // Change color to indicate firing
	ClampPlayerOnScreen(player);
}

// Move player down
void MoveDown(Player *player, float deltaTime)
{
	// Simple move down
	player->y += player->speed * deltaTime;
	player->color = GREEN; // Change color to default color
	ClampPlayerOnScreen(player);
}

// Move player left
void MoveLeft(Player *player, float deltaTime)
{
	// Simple move left
	player->x -= player->speed * deltaTime;
	player->color = GREEN; // Change color to default color
	ClampPlayerOnScreen(player);
}
// Move player right
void MoveRight(Player *player, float deltaTime)
{
	// Simple move right
	player->x += player->speed * deltaTime;
	player->color = GREEN; // Change color to default color
	ClampPlayerOnScreen(player);
}
// Player fires
void Fire(Player *player, float deltaTime)
{
	// Simple Recoil effect
	player->x -= RECOIL_SPEED * deltaTime;
	player->color = RED; // Change color to indicate firing
	ClampPlayerOnScreen(player);
}

void Jump(Player *player, float deltaTime)
{
	// Simple jump effect
	// Once per key press, so a fixed distance
	// deltaTime not required (unless you want to smoothly animate the jump over time)
	(void)deltaTime;
	player->y -= JUMP_HEIGHT;
	player->color = DARKGREEN; // Change color to indicate Jumping
	ClampPlayerOnScreen(player);
	TraceLog(LOG_INFO, "Player jumped at position (%.0f, %.0f)", player->x, player->y);
}

void JumpFire(Player *player, float deltaTime)
{
	// Simple jump and fire effect
	//deltaTime not required (unless you want to smoothly animate the jump over time)
	(void)deltaTime;
	player->y -= JUMP_HEIGHT;
	player->x -= 3.0f; // Recoil effect
	player->color = RED; // Change color to indicate firing
	ClampPlayerOnScreen(player);
	TraceLog(LOG_INFO, "Player jumped and fired at position (%.0f, %.0f)", player->x, player->y);
}