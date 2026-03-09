#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Drawable.h"
#include <memory>
#include <string>

class Manager;

class Item : public Drawable {

private:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	std::string name;
	std::string description;
	int value;
	glm::vec3 position;
	int amount;
	int StackSize;

public:
	//constructors
	Item(const std::string& name, std::shared_ptr<Shader> shader, glm::vec3 pos, int value = 0);
	virtual void setupMesh() = 0;

	//getters 
	std::string getName() const { return name; }
	std::string getDescription() const { return description; }
	int getValue() const { return value; }
	glm::vec3 getPosition() const { return position; }
	int getAmount() const { return amount; }
	int getStackSize() const { return StackSize; }

	//setters
	void setPosition(const glm::vec3& pos) { position = pos; }
	void setAmount(int newAmount) { amount = newAmount; }

	virtual void use(Manager& manager) const = 0; 

	//graphics
	void draw(const glm::mat4& view, const glm::mat4& projection) override;
};