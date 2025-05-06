#include "GameObject.h"

#include <array>

class GameGround : public GameObject
{
public:
	GameGround() {}
	GameGround(glm::vec3 position_, glm::vec3 size_);

	~GameGround();

	virtual void InitializeGameObject();
	virtual void InitializeGameObjectTextures(const char* file_);
	virtual void DrawGameObject();

	glm::vec3 position, size;

	Shader shader;

private:
	unsigned int cubeVAO, cubeVBO, cubeTexture;

	std::array<float, 206> cubeVertices;
};

