#ifndef MAINMENUOBJECT_H
#define MAINMENUOBJECT_H

#include "Shader.h"

class MainMenuObject
{
public:
	MainMenuObject() {}

	virtual ~MainMenuObject() {}

	virtual void InitializeMainMenuObject() = 0;
	virtual void InitializeMainMenuObjectTextures(const char* file_) = 0;
	virtual void DrawMainMenuObject() = 0;
};

#endif //MAINMENUOBJECT_H