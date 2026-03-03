#pragma once
#include <memory>
#include "Player.h"
#include "NPC.h"


class Cone : public Drawable {

private:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;

	glm::vec3 position;
	glm::vec3 direction;
	float radius;
	
	glm::vec3 colour;


	void setupMesh();

public:
	Cone(std::shared_ptr<Shader> shader, float radius = 2.f)
		: shader(shader), radius(radius)
	{
		Cone::setupMesh();
	}

	void update(Player& player, NPC& npc);

	void draw(const glm::mat4& view, const glm::mat4& projection) override;


};