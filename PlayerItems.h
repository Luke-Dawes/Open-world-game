#pragma once
#include "Item.h"
#include "Drawable.h"
#include "Player.h"
#include "GameManager.h"

class HealthPotion : public Item {

private:
	int healAmount = 5; 

public:
	HealthPotion(std::shared_ptr<Shader> shader, glm::vec3 pos, int healAmount);

	void use(Manager& manager) const override; //need to get a player refrence to this

	void setupMesh() override;
};

class DamagePotion : public Item {

private:
	int damageAmount = 5;

public:
	DamagePotion(std::shared_ptr<Shader>, glm::vec3 pos, int damage);

	void use(Manager& manager) const override;

	void setupMesh() override;
};