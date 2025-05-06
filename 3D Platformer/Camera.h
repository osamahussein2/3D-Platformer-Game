#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>

// Include the GLM header files (OpenGL Mathematics Library)
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
public:
	// Create static instances of camera vec3s so that we can easily use them inside of another class
	static glm::vec3 cameraPosition, cameraTarget, cameraDirection;
	static glm::vec3 cameraUp, cameraRight, cameraFront;
	static glm::vec3 Xaxis, Yaxis, Zaxis;

	static float yaw;
	static float pitch;

	static float fieldOfView;

	static bool constrainPitch;
};

#endif