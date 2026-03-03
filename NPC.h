#pragma once
#include "Drawable.h"
#include "Player.h"
#include <functional>

enum class NPCState {ANGRY, PASSIVE};

class NPC : public Drawable {

private:
	std::shared_ptr<Shader> shader;
	std::string name;
	NPCState state = NPCState::PASSIVE; //angry or passive
	glm::vec3 position;
	glm::vec3 velocity = glm::vec3(0.f);
	float speed = 5.f;
	float selfRadius = 1.f; //so the player and ai dont overlap
	float rangeRadius = 5.f; //range of which the ai will "agro" in

	//pathing - random coordinates that the AI will path to if its in passive mode
	std::vector<glm::vec3> randomPositions = {  //this needs changing
		glm::vec3(-13, 0, -28), glm::vec3(-17,0,-25), glm::vec3(-22, 0, -21), glm::vec3(-27, 0, -16), glm::vec3(-30, 0, -15), glm::vec3(-34,0,-10), glm::vec3(-36,0,-36),
		glm::vec3(-30,0,-10), glm::vec3(-10,0,-20), glm::vec3(-20,0,-10), glm::vec3(-10,0,-15), glm::vec3(-30,0,-10)
	};
	glm::vec3 currentTarget =  randomPositions[((int)glfwGetTime() * 10) % randomPositions.size()]; //get a random number which is below the size of random positions
	
	//collision detection
	glm::mat4 model;
	AABB globalAABB;

	//attack
	int maxHealth = 50;
	int health = 50;
	bool isAlive = true;
	int damage = 10;

public:

	std::shared_ptr<Mesh> mesh;

	NPC(const std::string& name, std::shared_ptr<Shader> shader, glm::vec3 position, std::shared_ptr<Mesh> mesh); //constructor

	void update(float deltaTime, Player& player); //applies any movement to the NPC

	void draw(const glm::mat4& view, const glm::mat4& projection) override; //draw it

	glm::vec3 getPosition() const { return position; } //get the positon of the NPC

	AABB getGlobalAABB() { return globalAABB; }

	void move(glm::vec3 pos);

	void takeDamage(int damage) { health = std::max(0, health - damage); if (health == 0) { isAlive = false; } }
	
	bool inline isNpcAlive() const { return isAlive; }

	void setRadiusRange(int range) { rangeRadius = range; }

	int getHealth() const { return health; }

	int getMaxHealth() const { return maxHealth; }

	void clearRandomPosition() { randomPositions = { position }; }

	inline int getDamage() { return damage; }
};

class EnemyStats {
	int damage; //class attribute set in constructor

public:
	EnemyStats(int damage) : //sets the damage
		damage(damage) 
	{}

	inline int getDamage() { return damage; }

};

class Wolf : public NPC, public EnemyStats { //needs NPC and stats
public:
	Wolf(const std::string& name, std::shared_ptr<Shader> shader, glm::vec3 position, int damage, std::shared_ptr<Mesh> pmesh)
		:
		EnemyStats(damage) , //uses the enemystats constructor
		NPC(name, shader, position, pmesh) //uss the NPC constructor
	{
		if (mesh->getLocalAABB().max != glm::vec3(1.f)) { //check to see if its calculated yet
			mesh->getLocalAABB().max.y = mesh->getLocalAABB().max.y * 0.5; //scale the max.y to a lower height
			mesh->getLocalAABB().max.z = mesh->getLocalAABB().max.z * 0.7; //scale the max z to a shorter length
			mesh->getLocalAABB().min.z = mesh->getLocalAABB().min.z * 0.5; //scale the min z to a shorter length
		}
	}

	
};


class Sheep : public NPC {

public:
	Sheep(const std::string& name, std::shared_ptr<Shader> shader, glm::vec3 position, std::shared_ptr<Mesh> pmesh)
		:
		NPC(name, shader, position, pmesh) //create an NPC object with a different mesh
	{
		setRadiusRange(0.f); //make the range which the ai will agro in 0


		if (mesh->getLocalAABB().max != glm::vec3(1.f)) {
			mesh->getLocalAABB().max.x = mesh->getLocalAABB().max.x * 0.5;
			mesh->getLocalAABB().min.x = mesh->getLocalAABB().min.x * 0.5;
			mesh->getLocalAABB().max.y = mesh->getLocalAABB().max.y * 0.5;
			mesh->getLocalAABB().min.y = mesh->getLocalAABB().min.y * 0.5;
		}

	}

};

struct dialogueNode {
	std::string text; //holds the text
	std::function<bool(std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec)> condition;
	//there is a function which returns bool and has two parameters
	//This lets me decide if i want to show the text depending on the bool
};

class FriendlyNPC : public NPC {

	void setupDialogue(); //create the dialogue
	std::vector<dialogueNode> speech; //store it
	

public:
	FriendlyNPC(const std::string& name, std::shared_ptr<Shader> shader, glm::vec3 position, std::shared_ptr<Mesh> mesh)
		:
		NPC(name, shader, position, mesh)
	{
		setRadiusRange(0.f); //make it non hostile
		clearRandomPosition(); //make it not move
		setupDialogue(); //create the dialogue
		
	}

	int npcDialogueIndex = 0; //keep an index

	auto getSpeech() const { return speech; }

	std::string speak(std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec); //makes the npc talk

	bool isTalking = false;
};

