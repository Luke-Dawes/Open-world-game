#include "Inventory.h"
#include <algorithm>

Inventory::Inventory(int maxSlots)
	: maxSlots(maxSlots) {}


void Inventory::addToInventory(std::shared_ptr<Item> newItem) {

	//if already in inventory
	for (auto& invItem : inventory) {
		if (invItem->getName() == newItem->getName()) {

			if (invItem->getAmount() == invItem->getStackSize()) { break; }

			invItem->setAmount(std::min(invItem->getAmount() + newItem->getAmount(), invItem->getStackSize()));
			return;
		}

	}
	//if not already in inventory
	if (inventory.size() < maxSlots) {
		inventory.push_back(newItem);
	}
	else {
		std::cout << "No space in inventory"; //display on screen somehow 
	}
}

void Inventory::removeItem(std::string name, int value) {

	std::erase_if(inventory, [&](auto& it) { //loop through inventory
		if (it->getName() == name) { //if its the correct item
			it->setAmount(it->getAmount() - value); //use the correct amount of items
			return it->getAmount() <= 0; //return true (to remove the item) if thers <0 items left
		}
		return false;
	});
}


bool Inventory::hasItem(const std::string& name) {
	
	for (auto& item : inventory) {
		if (item->getName() == name) {
			return true;
		}
	}
	return false;
}

