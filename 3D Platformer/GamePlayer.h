#include "GameObject.h"

#include <array>

class GamePlayer : public GameObject
{
public:
	GamePlayer() : position(vec3(0.0f)), velocity(vec3(0.0f)), size(vec3(0.0f)) {}
	GamePlayer(glm::vec3 position_, glm::vec3 velocity_, glm::vec3 size_);

	~GamePlayer();

	virtual void InitializeGameObject();
	virtual void InitializeGameObjectTextures(const char* file_);
	virtual void DrawGameObject();

	glm::vec3 position, size, velocity;

	Shader shader;

	bool isPlayerGrounded = false;

private:
	unsigned int cubeVAO, cubeVBO, cubeTexture;

	std::array<float, 206> cubeVertices;
};