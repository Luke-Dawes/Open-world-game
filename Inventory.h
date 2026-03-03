#pragma once

#include "Item.h"
#include "Drawable.h"
#include <memory>

class Inventory {

private:
	std::vector<std::shared_ptr<Item>> inventory;
	int maxSlots = 20;

	std::shared_ptr<Item> currentHelmet = nullptr;
	std::shared_ptr<Item> currentChest = nullptr;
	std::shared_ptr<Item> currentBoots = nullptr;
	std::shared_ptr<Item> currentItem = nullptr;


public:
	Inventory(int maxSlots = 20);

	const std::vector<std::shared_ptr<Item>>& getInventory() const { return inventory; }

	void addToInventory(std::shared_ptr<Item> newItem);

	void removeItem(std::string name, int value);

	bool hasItem(const std::string& name);

	std::shared_ptr<Item> getCurrentHelmet() {return currentHelmet; }
	std::shared_ptr<Item> getCurrentChest() {return currentChest;}
	std::shared_ptr<Item> getCurrentBoots() { return currentBoots; }
	std::shared_ptr<Item> getCurrentItem() { return currentItem; }
	
};