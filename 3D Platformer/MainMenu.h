#include "Menu.h"
#include "PlayerCube.h"
#include "House.h"

#include <vector>

enum Characters
{
	dubaiPlayer1, dubaiPlayer2
};

class MainMenu : public Menu
{
public:
	virtual ~MainMenu();

	static MainMenu* Instance();
	static void DeleteMainMenuInstance();

	virtual void InitializeMenu();
	virtual void UpdateMenu(float deltaTime_);
	virtual void RenderMenu();

	Characters playerCharacter = dubaiPlayer1;

private:
	MainMenu();

	void PlayerCubeCollision();
	bool CheckForDoorCollision(PlayerCube& player, House& door_);

	static MainMenu* mainMenuInstance;

	PlayerCube* playerCube;
	House* houseCube;

	vector<House*> doors;

	bool playerShaderSet, houseShaderSet, doorsShaderSet;

	//void PlayerCubeCollision();

	enum ShaderMaps
	{
		texture, invertedTexture
	};

	enum CubeTextures
	{
		dubai1, dubaiHouse
	};

	vec3 dubaiHousePos, dubaiHouseSize;

	bool switchedCharacter;
};
