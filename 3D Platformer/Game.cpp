#include "Game.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <iostream>
#include "Window.h"
#include "MainMenu.h"

Game* Game::gameInstance = nullptr;

Game::Game() : playerCube(new GamePlayer(vec3(0.0f, 0.0f, 2.0f), vec3(0.3f)))
{
}

Game::~Game()
{
	gameInstance = nullptr;

	delete playerCube;

	for (int i = 0; i < groundPlanes.size(); i++)
	{
		delete groundPlanes[i];
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
	Camera::cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

	Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (Camera::cameraPosition.y <= -1.3f)
	{
		Camera::cameraPosition.y = -1.3f;
	}

	// Initialize cubes
	playerCube->InitializeGameObject();

	if (MainMenu::Instance()->playerCharacter == dubaiPlayer1)
	{
		playerCube->InitializeGameObjectTextures("Textures/Dubai1.jpeg");
	}

	else if (MainMenu::Instance()->playerCharacter == dubaiPlayer2)
	{
		playerCube->InitializeGameObjectTextures("Textures/Dubai2.jpeg");
	}

	groundPlanes.push_back(new GameGround(vec3(0.0f, -1.5f, 0.06f), vec3(5.0f, 0.1f, 5.0f)));

	// Initialize ground planes for the game
	groundPlanes[0]->InitializeGameObject();

	// Don't set the shaders yet
	playerShaderSet = false;
	groundPlanesShaderSet = false;

	//ResourceManager::LoadShader("Shaders/Texture.vert", "Shaders/Texture.frag", texture);
}

void Game::UpdateGame(float deltaTime_)
{
	//cout << playerCube->position.x << ", " << playerCube->position.y << ", " << playerCube->position.z << endl;

	if (playerCube->position.y <= -3.0f)
	{
		playerCube->position = Camera::cameraPosition + Camera::cameraFront;

		playerFellOff = true;
	}

	if (!playerFellOff)
	{
		playerCube->position = Camera::cameraPosition + Camera::cameraFront;
	}

	else
	{
		Camera::cameraPosition = vec3(0.0f, 0.0f, 2.0f);
		playerFellOff = false;
	}

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

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS && playerCube->isPlayerGrounded &&
		jumping == false)
	{
		playerCube->isPlayerGrounded = false;
		jumping = true;
	}

	if (!playerCube->isPlayerGrounded && !playerFellOff && !jumping)
	{
		Camera::cameraPosition.y -= deltaTime_;
	}

	else if (playerCube->isPlayerGrounded && !playerFellOff && !jumping)
	{
		Camera::cameraPosition.y += 0.0f;
	}

	if (!playerCube->isPlayerGrounded && jumping)
	{
		float jumpHeight = 0.4f;
		float incrementJump = deltaTime_;

		// Move the camera up during the jump
		Camera::cameraPosition.y += incrementJump;

		// If the camera position's exceeds jump height, fall back down
		if (Camera::cameraPosition.y >= jumpHeight)
		{
			jumping = false;
		}
	}
	
	// Update player's collision with the ground once detected
	PlayerGroundCollision(*playerCube);
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

void Game::PlayerGroundCollision(GamePlayer& player)
{
	// First ground collision detection with the player
	if (player.position.x >= groundPlanes[0]->position.x - 2.65f &&
		player.position.x <= groundPlanes[0]->position.x + 2.65f &&
		player.position.y >= groundPlanes[0]->position.y - groundPlanes[0]->size.y - 0.1f &&
		player.position.y <= groundPlanes[0]->position.y + groundPlanes[0]->size.y + 0.1f &&
		player.position.z >= groundPlanes[0]->position.z - 2.58f &&
		player.position.z <= groundPlanes[0]->position.z + 2.58f && !jumping && !player.isPlayerGrounded)
	{
		player.isPlayerGrounded = true;
	}

	/* Make sure to detect the player being greater than the ground's position on any axes to set the is player grounded
	to false so that the player can fall down */
	else if (player.position.x < groundPlanes[0]->position.x - 2.65f ||
		player.position.x > groundPlanes[0]->position.x + 2.65f ||
		player.position.y < groundPlanes[0]->position.y - groundPlanes[0]->size.y - 0.1f ||
		player.position.y > groundPlanes[0]->position.y + groundPlanes[0]->size.y + 0.1f ||
		player.position.z < groundPlanes[0]->position.z - 2.58f ||
		player.position.z > groundPlanes[0]->position.z + 2.58f)
	{
		player.isPlayerGrounded = false;
	}
}