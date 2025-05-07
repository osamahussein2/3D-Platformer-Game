#ifndef GAME_H
#define GAME_H

#include "GamePlayer.h"
#include "GameGround.h"

#include <vector>

class Game
{
public:
	~Game();

	static Game* Instance();
	static void DeleteGameInstance();

	void InitializeGame();
	void UpdateGame(float deltaTime_);
	void RenderGame(float deltaTime_);

private:
	Game();

	bool PlayerGroundCollision(GamePlayer& player, GameGround& ground);

	void ModifyJumpHeight();

	static Game* gameInstance;

	GamePlayer* playerCube;
	std::vector<GameGround*> groundPlanes;

	bool playerShaderSet, groundPlanesShaderSet;

	bool jumping = false;

	float jumpHeight = 0.0f;
};

#endif*/ //