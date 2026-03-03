#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Drawable.h"
#include "AssimpMeshBuilder.h"
#include "WorldObjects.h"
#include <GLFW/glfw3.h>
#include "Inventory.h"
#include "Settings.h"
#include <algorithm>

class PlayerProjectile;

class Player : public Drawable {
private:

	std::shared_ptr<Settings> settings;

	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader; 
	std::shared_ptr<Inventory> inventory;

	//combat
	std::vector<std::shared_ptr<PlayerProjectile> > playerproj;

	float radius = 1.f;

	glm::vec3 position = glm::vec3(1.f, 0.1f, 1.f);

	float speed = 50.f;

	glm::vec3 velocity = glm::vec3(0.f);
	bool onGround = true;
	float jumpHeight = 5.f;
	float gravity = 9.81f;

	//movement
	void handlePhysics(float deltaTime);
	void handleInput(GLFWwindow* window, float deltaTime);
	void pickUpCoins(std::vector<std::shared_ptr<Coin>>& coins);

	int money = 0;

	int health = 50;

	int maxHealth = 50;
	bool alive = true;
	float lastDamageTime = 0;

	//rotations
	float yaw = 0.f;
	float rotationspeed = 90.f;
	glm::vec3 playerFront = glm::vec3(0.f,0.f,-5.f);

	//collision detection
	glm::mat4 model;
	AABB globalAABB;

	//attacking
	float attackDelay = 0.25f;
	float lastAttack;

	//interaction
	bool interact = false;

	std::string name;

	int damage = 20;

public:
	Player(std::string name, std::shared_ptr<Shader> s, std::shared_ptr<Inventory> I, std::shared_ptr<Settings> S);

	void draw(const glm::mat4& view, const glm::mat4& projection) override;

	void setupMesh();

	glm::vec3 getPlayerPosition() const { return position; }

	int getMoney() const { return money; }

	int getHealth() const { return health; }

	int getMaxHealth() const { return maxHealth; }

	std::shared_ptr<Inventory>& getInventory() { return inventory; }

	std::shared_ptr<Settings>& getSettings() { return settings; }

	void addToCurrentHealth(int value) { health = std::min(health + value, maxHealth); }

	void takeDamage(int value);

	float getYaw() { return yaw; }

	glm::vec3 getPlayerfront() { return playerFront; }

	void update(GLFWwindow* window, float dt, std::vector<std::shared_ptr<Coin>> coins); //wrapper for AABB, model, and input

	AABB getGlobalAABB() { return globalAABB; }

	void move(glm::vec3 pos);

	std::vector<std::shared_ptr<PlayerProjectile> >& getPlayerProj() { return playerproj; }

	void setLastAttack(float time) { lastAttack = time; }

	bool checkAlive() const { return alive; }

	bool getInteract() const { return interact; }

	void setPosition(glm::vec3 pos) { position = pos; }

	void setHealth(int value) { health = value; }

	void setYaw(float value) { yaw = value; }

	void setMoney(int value) { money = value; }

	void setName(std::string value) { name = value; }

	std::string getName() { return name; }

	void increaseDamage(int value) { damage += value; }


}; 

class PlayerProjectile : public Drawable {
private:
	std::shared_ptr<Shader> shader;

	//time which it will expire
	float expiraryTime;


	//how fast it moves - will be constant
	glm::vec3 velocity = glm::vec3(15.f, 0.f, 15.f);
	glm::vec3 forward;

	glm::vec3 position;

	AABB globalAABB;
	glm::vec3 model;

	//how much damage it will do
	int damage; 
public:

	bool isAlive = true;
	static std::shared_ptr<Mesh> mesh; //only loaded once
	
	PlayerProjectile(int pDamage, glm::vec3 pForward, glm::vec3 Position, std::shared_ptr<Shader> shader) 
		: damage(pDamage), forward(pForward), position(Position), shader(shader) //sets damage
	{
		expiraryTime = (float)glfwGetTime() + 2; //standard time of 3 seconds before it expires

		//this will make the velocity equal the forward direction. 
		//if i add the velocity to the position it will always go up or in a weird way beacuse of the y
		velocity *= forward;
	}

	~PlayerProjectile() = default;

	AABB getGlobalAABB() { return globalAABB; }

	void update(GLFWwindow* window, float dt, float time);

	void draw(const glm::mat4& view, const glm::mat4& projection) override;

	static void setupMesh();

};
