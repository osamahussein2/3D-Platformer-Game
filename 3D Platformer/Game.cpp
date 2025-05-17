#include "Game.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <iostream>
#include "Window.h"
#include "MainMenu.h"

Game* Game::gameInstance = nullptr;

Game::Game() : playerCube(new GamePlayer(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f), vec3(0.3f)))
{
}

Game::~Game()
{
	gameInstance = nullptr;

	delete playerCube;
	playerCube = nullptr;

	for (int i = 0; i < groundPlanes.size(); i++)
	{
		delete groundPlanes[i];
		groundPlanes[i] = nullptr;
	}

	groundPlanes.clear();
}

Game* Game::Instance()
{
	if (gameInstance == nullptr)
	{
		gameInstance = new Game();
		return gameInstance;
	}

	return gameInstance;
}

void Game::InitializeGame()
{
	// Initialize main menu camera
	Camera::cameraPosition = glm::vec3(0.0f, 0.0f, 2.0f);

	Camera::cameraFront = glm::vec3(0.0f, 0.5f, -1.0f);
	Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Initialize cubes
	playerCube->InitializeGameObject();

	if (MainMenu::Instance()->playerCharacter == dubaiPlayer1)
	{
		lastSavedPlayerCharacter = dubaiPlayer1;
		playerCube->InitializeGameObjectTextures("Textures/Dubai1.jpeg");
	}

	else if (MainMenu::Instance()->playerCharacter == dubaiPlayer2)
	{
		lastSavedPlayerCharacter = dubaiPlayer2;
		playerCube->InitializeGameObjectTextures("Textures/Dubai2.jpeg");
	}

	groundPlanes.push_back(new GameGround(vec3(0.0f, -1.5f, 0.06f), vec3(5.0f, 0.1f, 5.0f)));
	groundPlanes.push_back(new GameGround(vec3(0.0f, -1.0f, -4.0f), vec3(2.0f, 0.1f, 2.0f)));

	for (int i = 0; i < groundPlanes.size(); i++)
	{
		// Initialize ground planes for the game
		groundPlanes[i]->InitializeGameObject();

		if (MainMenu::Instance()->playerCharacter == dubaiPlayer1)
		{
			groundPlanes[i]->InitializeGameObjectTextures("Textures/Dubai1.jpeg");
		}

		else if (MainMenu::Instance()->playerCharacter == dubaiPlayer2)
		{
			groundPlanes[i]->InitializeGameObjectTextures("Textures/Dubai2.jpeg");
		}
	}

	// Don't set the shaders yet
	playerShaderSet = false;
	groundPlanesShaderSet = false;

	//ResourceManager::LoadShader("Shaders/Texture.vert", "Shaders/Texture.frag", texture);
}

void Game::UpdateGame(float deltaTime_)
{
	//cout << playerCube->position.x << ", " << playerCube->position.y << ", " << playerCube->position.z << endl;

	playerCube->position = Camera::cameraPosition + Camera::cameraFront;

	// Detect input for player movement
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		// Move the camera towards the screen when W is pressed
		Camera::cameraPosition += 1.0f * deltaTime_ * Camera::cameraFront;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		// Move the camera away from the screen when S key is pressed
		Camera::cameraPosition -= 1.0f * deltaTime_ * Camera::cameraFront;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		// Move the camera left when A key is pressed
		Camera::cameraPosition -= glm::normalize(glm::cross(Camera::cameraFront, Camera::cameraUp)) *
			1.0f * deltaTime_;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		// Move the camera right when D key is pressed
		Camera::cameraPosition += glm::normalize(glm::cross(Camera::cameraFront, Camera::cameraUp)) *
			1.0f * deltaTime_;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		Window::Instance()->previousState = game;
		Window::Instance()->currentState = mainMenu;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS && 
		PlayerGroundCollision(*playerCube, *groundPlanes[0]) && jumping == false)
	{
		//playerCube->isPlayerGrounded = false;
		jumping = true;
	}

	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS &&
		PlayerGroundCollision(*playerCube, *groundPlanes[1]) && jumping == false)
	{
		//playerCube->isPlayerGrounded = false;
		jumping = true;
	}

	// Update the player's falling velocity when not landed on ground yet
	if (!PlayerGroundCollision(*playerCube, *groundPlanes[0]) && !jumping)
	{
		Camera::cameraPosition.y -= deltaTime_;
	}

	// Update player's collision with the ground once detected
	else if (PlayerGroundCollision(*playerCube, *groundPlanes[0]) && !jumping)
	{
		Camera::cameraPosition.y += 0.0f;

		// If player's cube y position is less than the first ground platform y position
		if (playerCube->position.y <= -1.3f)
		{
			// Don't move the camera
			if (Camera::cameraPosition.y <= -1.1f) Camera::cameraPosition.y = -1.1f;
			playerCube->position.y = -1.3f;
		}
	}

	if (PlayerGroundCollision(*playerCube, *groundPlanes[1]) && !jumping)
	{
		Camera::cameraPosition.y += 0.0f;

		// If player's cube y position is less than the second ground platform y position
		if (playerCube->position.y <= -0.7f)
		{
			// Don't move the camera
			if (Camera::cameraPosition.y <= -0.5f) Camera::cameraPosition.y = -0.5f;
			playerCube->position.y = -0.7f;
		}
	}

	/* Modify the player's jump height depending on which ground the player collided with to make sure
	their jump height is consistent with the rest of the platforms */
	ModifyJumpHeight();

	for (int i = 0; i < groundPlanes.size(); i++)
	{
		if (!PlayerGroundCollision(*playerCube, *groundPlanes[i]) && jumping)
		{
			float incrementJump = deltaTime_;

			// Move the camera up during the jump
			Camera::cameraPosition.y += incrementJump;

			// If the camera position's exceeds jump height, fall back down
			if (Camera::cameraPosition.y >= jumpHeight)
			{
				jumping = false;
			}
		}
	}
}

void Game::RenderGame(float deltaTime)
{
	//std::cout << playerCube->position.x << ", " << playerCube->position.y << ",  " <<
		//playerCube->position.z << std::endl;

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the
		window (unless if the application loses focus or quits )*/
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* As soon as we register the callback function with GLFW each time the mouse moves while the window has focus on
	the cursor, this MouseCallback function below will get called. */
	glfwSetCursorPosCallback(glfwGetCurrentContext(), Window::MouseCallback);

	if (playerShaderSet == false)
	{
		playerCube->shader.InitializeShader("Shaders/Texture.vert", "Shaders/Texture.frag");
		playerShaderSet = true;
	}

	else if (playerShaderSet == true)
	{
		playerCube->DrawGameObject();
	}

	if (groundPlanesShaderSet == false)
	{
		for (int i = 0; i < groundPlanes.size(); i++)
		{
			groundPlanes[i]->shader.InitializeShader("Shaders/Texture.vert", "Shaders/Texture.frag");
		}
		groundPlanesShaderSet = true;
	}

	else if (groundPlanesShaderSet == true)
	{
		for (int i = 0; i < groundPlanes.size(); i++)
		{
			groundPlanes[i]->DrawGameObject();
		}
	}
}

void Game::DeleteGameInstance()
{
	if (gameInstance) {
		delete gameInstance;
		gameInstance = nullptr;
	}
	else {
		std::cerr << "gameInstance is not valid to delete\n";
	}
}

bool Game::PlayerGroundCollision(GamePlayer& player, GameGround& ground)
{
	// First ground collision detection with the player
	if (player.position.x >= ground.position.x - ground.size.x / 1.9f &&
		player.position.x <= ground.position.x + ground.size.x / 1.9f &&
		player.position.y >= ground.position.y - ground.size.y - 0.1f &&
		player.position.y <= ground.position.y + ground.size.y + 0.1f &&
		player.position.z >= ground.position.z - ground.size.z / 1.9f &&
		player.position.z <= ground.position.z + ground.size.z / 1.9f && !jumping)
	{
		//player.isPlayerGrounded = true;
		return true;
	}

	/* Make sure to detect the player being greater than the ground's position on any axes to set the is player grounded
	to false so that the player can fall down */
	else if (player.position.x < ground.position.x - ground.size.x / 1.9f ||
		player.position.x > ground.position.x + ground.size.x / 1.9f ||
		player.position.y < ground.position.y - ground.size.y - 0.1f ||
		player.position.y > ground.position.y + ground.size.y + 0.1f ||
		player.position.z < ground.position.z - ground.size.z / 1.9f ||
		player.position.z > ground.position.z + ground.size.z / 1.9f)
	{
		//player.isPlayerGrounded = false;
		return false;
	}

	return false;
}

void Game::ModifyJumpHeight()
{
	if (PlayerGroundCollision(*playerCube, *groundPlanes[0]))
	{
		// First ground plane height is -1.5f
		jumpHeight = 0.2f;
	}

	if (PlayerGroundCollision(*playerCube, *groundPlanes[1]))
	{
		// Second ground plane height is -1.0f
		jumpHeight = 0.7f;
	}

	for (int i = 0; i < groundPlanes.size(); i++)
	{
		if (!PlayerGroundCollision(*playerCube, *groundPlanes[i]))
		{
			// Respawn the player upon falling out of level bounds
			if (Camera::cameraPosition.y <= -3.0f)
			{
				Camera::cameraPosition = vec3(0.0f, 0.0f, 2.0f);
			}
		}
	}
}
