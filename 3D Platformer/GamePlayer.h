#include "GameObject.h"

#include <array>

class GamePlayer : public GameObject
{
public:
	GamePlayer() {}
	GamePlayer(glm::vec3 position_, glm::vec3 size_);

	~GamePlayer();

	virtual void InitializeGameObject();
	virtual void InitializeGameObjectTextures(const char* file_);
	virtual void DrawGameObject();

	glm::vec3 position, size;

	Shader shader;

	bool isPlayerGrounded = false;;

private:
	unsigned int cubeVAO, cubeVBO, cubeTexture;

	std::array<float, 206> cubeVertices;
};