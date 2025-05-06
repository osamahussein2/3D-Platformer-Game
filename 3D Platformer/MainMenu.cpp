#include "MainMenu.h"
#include "Camera.h"
#include "ResourceManager.h"
#include <iostream>
#include "Window.h"

MainMenu* MainMenu::mainMenuInstance = nullptr;

MainMenu::MainMenu() : Menu(), playerCube(new PlayerCube(vec3(0.0f, 0.0f, 2.0f), vec3(0.3f))),
houseCube(new House(vec3(0.0f, 0.0f, 2.5f), vec3(5.0f)))
{
}

MainMenu::~MainMenu()
{
	mainMenuInstance = nullptr;

	delete playerCube, houseCube;

	for (int i = 0; i < doors.size(); i++)
	{
		delete doors[i];
	}
}

MainMenu* MainMenu::Instance()
{
	if (mainMenuInstance == nullptr)
	{
		mainMenuInstance = new MainMenu();
		return mainMenuInstance;
	}

	return mainMenuInstance;
}

void MainMenu::InitializeMenu()
{
	// Initialize main menu camera
	Camera::cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

	dubaiHousePos = glm::vec3(0.0f, 0.0f, 2.5f);
	dubaiHouseSize = glm::vec3(5.0f);

	Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Initialize cubes
	playerCube->InitializeMainMenuObject();

	if (playerCharacter == NULL)
	{
		playerCharacter = dubaiPlayer1;
	}

	switchedCharacter = false;

	if (playerCharacter == dubaiPlayer1) playerCube->InitializeMainMenuObjectTextures("Textures/Dubai1.jpeg");
	else if (playerCharacter == dubaiPlayer2) playerCube->InitializeMainMenuObjectTextures("Textures/Dubai2.jpeg");

	houseCube->InitializeMainMenuObject();
	houseCube->InitializeMainMenuObjectTextures("Textures/DubaiHouse.png");

	// Play door
	doors.push_back(new House(vec3(0.0f, -1.5f, 0.06f), vec3(1.0f, 2.0f, 0.1f)));

	doors[0]->InitializeMainMenuObject();
	doors[0]->InitializeMainMenuObjectTextures("Textures/DubaiDoorPlay.png");

	// Quit door
	doors.push_back(new House(vec3(-2.44f, -1.5f, 3.0f), vec3(0.1f, 2.0f, 1.0f)));

	doors[1]->InitializeMainMenuObject();
	doors[1]->InitializeMainMenuObjectTextures("Textures/DubaiDoorQuit.png");

	// Switch characters door
	doors.push_back(new House(vec3(2.44f, -1.5f, 3.0f), vec3(0.1f, 2.0f, 1.0f)));

	doors[2]->InitializeMainMenuObject();
	doors[2]->InitializeMainMenuObjectTextures("Textures/DubaiDoorSwitchCharacters.png");

	// Don't set the shaders yet
	playerShaderSet = false;
	houseShaderSet = false;
	doorsShaderSet = false;

	//ResourceManager::LoadShader("Shaders/Texture.vert", "Shaders/Texture.frag", texture);
}

void MainMenu::UpdateMenu(float deltaTime_)
{
	// Move the player's cube with the camera
	playerCube->position = Camera::cameraPosition + Camera::cameraFront;

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

	

	// Update player's collisions once detected
	PlayerCubeCollision();
}

void MainMenu::RenderMenu()
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
		playerCube->DrawMainMenuObject();
	}

	if (houseShaderSet == false)
	{
		houseCube->shader.InitializeShader("Shaders/InvertedTexture.vert", "Shaders/InvertedTexture.frag");

		houseShaderSet = true;
	}

	else if (houseShaderSet == true)
	{
		houseCube->DrawMainMenuObject();
	}

	if (doorsShaderSet == false)
	{
		for (House* door : doors)
		{
			door->shader.InitializeShader("Shaders/Texture.vert", "Shaders/Texture.frag");
		}

		doorsShaderSet = true;
	}

	else if (doorsShaderSet == true)
	{
		for (House* door : doors)
		{
			door->DrawMainMenuObject();
		}
	}
}

void MainMenu::DeleteMainMenuInstance()
{
	if (mainMenuInstance) {
		delete mainMenuInstance;
		mainMenuInstance = nullptr;
	}
	else {
		std::cerr << "mainMenuInstance is not valid to delete\n";
	}
}

void MainMenu::PlayerCubeCollision()
{
	// If player collided with the play door
	if (CheckForDoorCollision(*playerCube, *doors[0]))
	{
		Window::Instance()->areResourcesDeallocated = false;
		Window::Instance()->state = game;
	}

	// If player collided with the quit door
	if (CheckForDoorCollision(*playerCube, *doors[1]))
	{
		glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
	}

	// If player collided with the switch characters door
	if (CheckForDoorCollision(*playerCube, *doors[2]))
	{
		if (playerCharacter == dubaiPlayer1 && switchedCharacter == false &&
			glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			playerCharacter = dubaiPlayer2;
			playerCube->InitializeMainMenuObjectTextures("Textures/Dubai2.jpeg");

			switchedCharacter = true;
		}

		if (playerCharacter == dubaiPlayer2 && switchedCharacter == false &&
			glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ENTER) == GLFW_PRESS)
		{
			playerCharacter = dubaiPlayer1;
			playerCube->InitializeMainMenuObjectTextures("Textures/Dubai1.jpeg");

			switchedCharacter = true;
		}

		std::cout << "Collided with door!" << endl;
	}

	else
	{
		switchedCharacter = false;
	}
}

bool MainMenu::CheckForDoorCollision(PlayerCube& player, House& door_)
{
	if (player.position.x >= door_.position.x &&
		player.position.x <= door_.position.x + door_.size.x &&
		player.position.y >= door_.position.y &&
		player.position.y <= door_.position.y + door_.size.y &&
		player.position.z >= door_.position.z &&
		player.position.z <= door_.position.z + door_.size.z)
	{
		return true;
	}

	return false;
}
