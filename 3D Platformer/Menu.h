#ifndef MENU_H
#define MENU_H

#include <vector>

class Menu
{
protected:
	Menu() {}
	virtual ~Menu() {}

	virtual void InitializeMenu() = 0;
	virtual void UpdateMenu(float deltaTime) = 0;
	virtual void RenderMenu() = 0;
};

#endif