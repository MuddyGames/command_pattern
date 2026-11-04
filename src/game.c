#include "game.h"

// Initialise Game Data
void InitGame(GameData *data)
{
	// Initialise Player
	Player player = {
		SCREEN_WIDTH / 2,	   // Center X
		SCREEN_HEIGHT / 2,	   // Center Y
		DEFAULT_PLAYER_RADIUS, // Radius
		100.0f,				   // Speed
		100,				   // Health
		0.0f,				   // Breath timer accumulator
		GREEN				   // Color
	};

	// Set player
	data->player = player;

	// Initialise HUD
	InitHud(&data->hud);

	// Create Mediator: it connects the player and the HUD
	data->mediator = CreateMediator(&data->player, &data->hud);
	data->activeCommand = NONE;

	// Initialise Input Manage
	InitInputManager();
}

// Update Game Data
void UpdateGame(GameData *data, float deltaTime)
{
	// Poll input and Handle Command
	data->activeCommand = PollInput();
	MediatorHandleCommand(data->mediator, data->activeCommand, deltaTime);
}

// Draw Game Data
void DrawGame(const GameData *data)
{
	// Drawing Player and Position Data
	Vector2 position = {data->player.x, data->player.y};

	DrawCircleV(position, data->player.r, data->player.color);

	// Drawing HUD
	DrawHud(&data->hud);
}

// Close Game and free resources
void CloseGame(GameData *data)
{
	TraceLog(LOG_INFO, "Game Closed!");

	// Free Mediator
	if (data->mediator)
	{
		free(data->mediator);
		data->mediator = NULL;
	}

	free(data); // Free game data memory
}