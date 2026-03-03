#pragma once
#include <memory>
#include <vector>
#include "Player.h"
#include "Inventory.h"
#include "NPC.h"
#include "Camera.h"
#include "WorldObjects.h"
#include "PlayerItems.h"

namespace SaveLoad { void load(Manager* manager); } //forward declare that this is a thing

enum class Gamestate;
class Menu;

class Manager {
private:
	std::unique_ptr<Player> player;
	std::shared_ptr<Inventory> inventory; //need to work out if only gameManager holds it or if player also holds it 
	std::unique_ptr<Cube> cube; 
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Floor> floor;
	std::shared_ptr<Menu> menu; //this will be shared between this and the main file 
	std::vector<std::shared_ptr<Coin> > coinsVec;
	std::vector<std::shared_ptr<NPC> > NPCVec;
	std::shared_ptr<Settings> settings;
	std::vector<std::shared_ptr<Sheep> > sheepVec;
	std::vector<std::shared_ptr<WorldObjects> > worldObjectsVec;
	std::shared_ptr<deathScreen> DeathScreen;
	std::shared_ptr<FriendlyNPC> friendlyNPC;
	

#ifdef _DEBUG
	std::unique_ptr<redArrow> redarrow;
	std::unique_ptr<greenArrow> greenarrow;
	std::vector<std::shared_ptr<AABBorder> > NPCWireFrameVec;
	std::shared_ptr<AABBorder> playerFrame;
	std::vector<std::shared_ptr<AABBorder> > coinWireFrameVec;
	std::vector<std::shared_ptr<AABBorder> > sheepWireFrameVec;
	std::shared_ptr<AABBorder> houseFrame;
#endif // _DEBUG

	
	

public:

	bool shop = false;

	bool initGame(std::shared_ptr<Camera> preCamera, std::string name);

	Manager(std::shared_ptr<Camera> cam, std::string name, bool savedAccount = false)
	{

		if (!initGame(cam, name)) throw std::runtime_error("error loading game");
		if (savedAccount) SaveLoad::load(this);
	}

	void mainLoop(GLFWwindow* window);

	std::unique_ptr<Player>& getPlayer() { return player; }
	std::shared_ptr<Inventory>& getInventory() { return inventory; }
	std::shared_ptr<Camera>& getCamera() { return camera; }
	std::vector<std::shared_ptr<NPC> >& getNPC() { return NPCVec; }
	std::shared_ptr<Menu>& getMenu() { return menu; }
	std::vector<std::shared_ptr<Coin> >& getCoinVector() { return coinsVec; }
	std::vector<std::shared_ptr<Sheep> >& getSheep() { return sheepVec; }

};