#include "Window.h"
#include "MainMenu.h"
#include "Camera.h"
#include "Game.h"

float lastFrame = 0.0f;

Window* Window::windowInstance = nullptr;

float Window::lastPositionX = 600;
float Window::lastPositionY = 450;

// Initialize the first time mouse input to true since the mouse cursor will be immediately focused in OpenGL window
extern bool firstTimeMouseReceivesInput = true;

Window::Window() : openGLwindow(NULL), areResourcesDeallocated(true), state(mainMenu)
{
}

Window::~Window()
{
    MainMenu::Instance()->~MainMenu();

    // Close GLFW window
    glfwTerminate();
}

Window* Window::Instance()
{
    if (windowInstance == nullptr)
    {
        windowInstance = new Window();
        return windowInstance;
    }

    return windowInstance;
}

void Window::InitializeWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window to render it to the screen
    openGLwindow = glfwCreateWindow(width, height, title, monitor, share);

    // If our GLFWwindow is NULL, write to the console the window cannot be created and close down GLFW library
    if (openGLwindow == NULL)
    {
        cout << "GLFW Window cannot be created!" << endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(openGLwindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "GLAD cannot be initialized!" << endl;
    }

    glfwSetKeyCallback(openGLwindow, KeyCallback);
    glfwSetFramebufferSizeCallback(openGLwindow, FrameBufferSizeCallback);

    MainMenu::Instance()->InitializeMenu();
}

void Window::UpdateWindow()
{
    float currentFrame = glfwGetTime(); // Get the current time

    // deltaTime is the time between current frame and last frame
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame; // Get the time of the last frame

    glfwPollEvents(); // Waits for any input by the user and processes it in real-time

    switch (state)
    {
    case mainMenu:
        if (!areResourcesDeallocated)
        {
            break;
        }
        else if (areResourcesDeallocated)
        {
            MainMenu::Instance()->UpdateMenu(deltaTime);
            MainMenu::Instance()->RenderMenu();
            break;
        }


    case game:
        if (!areResourcesDeallocated)
        {
            Game::Instance()->InitializeGame();
            MainMenu::Instance()->~MainMenu();

            areResourcesDeallocated = true;
        }

        else if (areResourcesDeallocated)
        {
            Game::Instance()->UpdateGame(deltaTime);
            Game::Instance()->RenderGame(deltaTime);
            break;
        }

    default:
        break;
    }

    glfwSwapBuffers(openGLwindow); // Removing this will throw an exception error or nothing will pop up
}

void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// To calculate the pitch and yaw values, we must tell GLFW to listen to mouse-movement events
void Window::MouseCallback(GLFWwindow* window, double positionX, double positionY)
{
	// If the mouse receives input for the first time, update the initial mouse positions to the x and y positions
	if (firstTimeMouseReceivesInput == true)
	{
		lastPositionX = positionX;
		lastPositionY = positionY;

		firstTimeMouseReceivesInput = false; // Make this false afterwards to prevent long movement jumps
	}

	/* In order to find the mouse's offset from last frame, we have to store the last mouse x and y positions in the
	application. */
	float mouseOffsetX = positionX - lastPositionX;
	float mouseOffsetY = lastPositionY - positionY; // This has to be reversed since y ranges bottom to top

	lastPositionX = positionX;
	lastPositionY = positionY;

	/* We need to multiply the mosue offset values by the mouse sensitivity value here or else the mouse movement would
	be too strong. */
	const float mouseSensitivity = 0.1f;
	mouseOffsetX *= mouseSensitivity;
	mouseOffsetY *= mouseSensitivity;

	// Add the mouse offset x value to yaw and the mouse offset y value to pitch
	Camera::yaw += mouseOffsetX;
	Camera::pitch += mouseOffsetY;

	/* We need to restrict the pitch value not to exceed 89 and not to go below -89 to prevent the LookAt view matrix
	from flipping. */
	if (Camera::constrainPitch == true)
	{
		if (Camera::pitch >= 89.0f)
		{
			Camera::pitch = 89.0f;
		}

		else if (Camera::pitch <= -89.0f)
		{
			Camera::pitch = -89.0f;
		}
	}

	Camera::cameraDirection = glm::vec3(cos(glm::radians(Camera::yaw)) * cos(glm::radians(Camera::pitch)),
		sin(glm::radians(Camera::pitch)),
		sin(glm::radians(Camera::yaw) * cos(glm::radians(Camera::pitch))));

	Camera::cameraFront = glm::normalize(Camera::cameraDirection);
}
