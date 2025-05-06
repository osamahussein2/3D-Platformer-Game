#include "MainMenuObject.h"

#include <array>

class House : public MainMenuObject
{
public:
	House() {}
	House(glm::vec3 position_, glm::vec3 size_);

	~House();

	virtual void InitializeMainMenuObject();
	virtual void InitializeMainMenuObjectTextures(const char* file_);
	virtual void DrawMainMenuObject();

	glm::vec3 position, size;

	Shader shader;

private:
	unsigned int cubeVAO, cubeVBO, cubeTexture;

	std::array<float, 206> cubeVertices;
};