#include "MainMenuObject.h"

#include <array>

class PlayerCube : public MainMenuObject
{
public:
	PlayerCube() {}
	PlayerCube(glm::vec3 position_, glm::vec3 size_);

	~PlayerCube();

	virtual void InitializeMainMenuObject();
	virtual void InitializeMainMenuObjectTextures(const char* file_);
	virtual void DrawMainMenuObject();

	glm::vec3 position, size;

	Shader shader;

private:
	unsigned int cubeVAO, cubeVBO, cubeTexture;

	std::array<float, 206> cubeVertices;
};