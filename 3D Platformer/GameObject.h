#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Shader.h"

class GameObject
{
public:
	GameObject() {}

	virtual ~GameObject() {}

	virtual void InitializeGameObject() = 0;
	virtual void InitializeGameObjectTextures(const char* file_) = 0;
	virtual void DrawGameObject() = 0;
};

#endif //GAMEOBJECT_H