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

	void PlayerGroundCollision(GamePlayer& player, GameGround& ground);

	static Game* gameInstance;

	GamePlayer* playerCube;
	std::vector<GameGround*> groundPlanes;

	bool playerShaderSet, groundPlanesShaderSet;

	bool playerFellOff = false;
	bool jumping = false;
};

#endif*/ //