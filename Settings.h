#pragma once
#include <GLFW/glfw3.h>


class Settings {

	int Forwards = GLFW_KEY_W;
	int Backwards = GLFW_KEY_S;
	int Left = GLFW_KEY_A;
	int Right = GLFW_KEY_D;
	int Pause = GLFW_KEY_M;
	int Inventory = GLFW_KEY_I;
	int Jump = GLFW_KEY_SPACE;

public:
	Settings() = default;

	void setForwards(int key) { Forwards = key; }
	void setBackwards(int key) { Backwards = key; }
	void setLeft(int key) { Left = key; }
	void setRight(int key) { Right = key; }
	void setPause(int key) { Pause = key; }
	void setInventory(int key) { Inventory = key; }
	void setJump(int key) { Jump = key; }

	inline int getForwards() const { return Forwards; }
	inline int getBackwards() const { return Backwards; }
	inline int getLeft() const { return Left; }
	inline int getRight() const { return Right; }
	inline int getPause() const { return Pause; }
	inline int getInventory() const { return Inventory; }
	inline int getJump() const { return Jump; }

};