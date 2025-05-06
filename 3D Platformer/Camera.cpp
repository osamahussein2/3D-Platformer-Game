#include "Camera.h"

// ALWAYS declare the static variables and objects above the class functions
float Camera::yaw = -90.0f;
float Camera::pitch = 0.0f;

glm::vec3 Camera::cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);

// The camera position is a vector in world space that points to the camera's position
glm::vec3 Camera::cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

glm::vec3 Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 Camera::cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

float Camera::fieldOfView = 45.0f;

glm::vec3 Camera::Zaxis = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::Xaxis = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::Yaxis = glm::vec3(0.0f, 0.0f, 0.0f);

bool Camera::constrainPitch = true;