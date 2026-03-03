#include "PlayerItems.h"


HealthPotion::HealthPotion(std::shared_ptr<Shader> shader, glm::vec3 pos, int healAmount) 
	: Item("Health potion", shader, pos) , healAmount(healAmount) 
	{ 
		HealthPotion::setupMesh();
	}

void HealthPotion::use(Manager& manager) const {
	manager.getPlayer()->addToCurrentHealth(healAmount);
	manager.getPlayer()->getInventory()->removeItem("Health potion", 1);
}


void HealthPotion::setupMesh()
{
	///add later - mesh for health potion, but it will crash if i run it without adding this
}

DamagePotion::DamagePotion(std::shared_ptr<Shader> shader, glm::vec3 pos, int damageAmount) 
	: Item("Damage potion", shader, pos) , damageAmount(damageAmount) 
	{
		DamagePotion::setupMesh();
	}

void DamagePotion::use(Manager& manager) const {
	manager.getPlayer()->increaseDamage(damageAmount);
	manager.getPlayer()->getInventory()->removeItem("Damage potion", 1);
}

void DamagePotion::setupMesh() {}