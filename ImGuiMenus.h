#pragma once
#include "Main.h"
#include "Player.h"
#include "GameManager.h"

class NPC;

class Menu {

public:
	Menu() = default;

	Gamestate pauseMenu(GLFWwindow* window);
	Gamestate mainMenu(GLFWwindow* window, Manager* m);
	Gamestate settingsMenu(Player& player, GLFWwindow* window);
	Gamestate inventoryMenu(Manager& manager);
	Gamestate questMenu();
	Gamestate DeathScreen(const std::string& reason);
	void HUD(const Player& player, const std::vector<std::shared_ptr<Sheep> >& sheepvec, const std::vector<std::shared_ptr<NPC> >& npcvec);
	
	bool TalkToNPC(GLFWwindow* window, FriendlyNPC& npc, Manager& m);

	bool Shop(GLFWwindow* window, Manager& m);
};

extern int lastKeyPressed;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
		lastKeyPressed = key;
}