#pragma once

#include "Mesh.h"
#include "Drawable.h"
#include "Shader.h"
#include "AssimpMeshBuilder.h"
#include <memory>

class Floor : public Drawable {
private:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;

public:
	Floor(float size = 50.f, std::shared_ptr<Shader> parameterShader = nullptr);

	void draw(const glm::mat4& view, const glm::mat4& projection) override;

};

class Cube : public Drawable {

private:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	

public:
	Cube(std::shared_ptr<Shader> s);

	void draw(const glm::mat4& view, const glm::mat4& projection) override;

	void setupMesh();

};

class Coin : public Drawable {

private:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	glm::vec3 position;
	bool collected = false;
	float radius = 0.5f;
	int value = 5;

	glm::mat4 model;
	AABB globalAABB;

public:
	Coin(std::shared_ptr<Shader> s, glm::vec3 pos);

	void draw(const glm::mat4& view, const glm::mat4& projection) override;

	void setupMesh();

	bool isCollected() const { return collected; }

	glm::vec3 getPosition() const { return position; }

	float getRadius() const { return radius; }

	void collect() { collected = true; }

	int getValue() const { return value; }

	AABB getGlobalAABB() { return globalAABB; }

	void update();
};

class WorldObjects : public Drawable {
	std::shared_ptr<Shader> shader;
	std::unique_ptr<Mesh> mesh;

	//position of the object
	glm::vec3 position;

	//things for collision detection
	AABB globalAABB;
	glm::mat4 model;
	float scaleOfAABB;
	float actualScale;
	float rotation; //use this ====================================

public: 

	WorldObjects(const std::string& path, std::shared_ptr<Shader> shader, glm::vec3 position, float scaleOfAABB = 1.f, float actualScale = 1.f, float rotation = 0.f) 
		: shader(shader), position(position), scaleOfAABB(scaleOfAABB), actualScale(actualScale), rotation(rotation)
	{
		mesh = loadModel(path);
		setGlobalAABB();
	}

	AABB getGlobalAABB() { return globalAABB; }

	void draw(const glm::mat4& view, const glm::mat4& projection) override;

	void setGlobalAABB();

};






//debugging 

class Player;

class redArrow : public Drawable { ///keeping until i fix the player rotations aswell 
public:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 direction = glm::vec3(0.f);

	redArrow(std::shared_ptr<Shader> shader) :
		shader(shader)
	{
		setupMesh();
	}

	void update(Player& player);

	void setupMesh() {
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned int> indices;

		// Main line (white)
		vertices.push_back({ glm::vec3(0.0f), glm::vec4(1.0f) });
		vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1.0f) });

		indices.push_back(0);
		indices.push_back(1);

		// Arrowhead (red)
		vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1,0,0,1) });
		vertices.push_back({ glm::vec3(0.1f, 0.0f, 0.8f), glm::vec4(1,0,0,1) });
		indices.push_back(2);
		indices.push_back(3);

		vertices.push_back({ glm::vec3(0.0f, 0.0f, 1.0f), glm::vec4(1,0,0,1) });
		vertices.push_back({ glm::vec3(-0.1f, 0.0f, 0.8f), glm::vec4(1,0,0,1) });
		indices.push_back(4);
		indices.push_back(5);

		mesh = std::make_unique<Mesh>(vertices, indices);
	}

	void draw(const glm::mat4& view, const glm::mat4& projection) override {
		shader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);

		// Rotate arrow to point along direction
		if (glm::length(direction) > 0.0001f) {
			glm::vec3 defaultDir(0.0f, 0.0f, 1.0f);
			glm::vec3 axis = glm::cross(defaultDir, direction);
			float angle = acos(glm::dot(defaultDir, direction));
			if (glm::length(axis) > 0.0001f)
				model = glm::rotate(model, angle, glm::normalize(axis));
		}

		model = glm::scale(model, glm::vec3(1.f));
		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		mesh->draw();
	}

};


class greenArrow : public Drawable {
public:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 direction = glm::vec3(0.f);
	float rotationYaw = 0.f;

	greenArrow(std::shared_ptr<Shader> shader) :
		shader(shader)
	{
		setupMesh();
	}

	void update(const glm::vec3& playerPos, const glm::vec3& cameraFront); 

	void setupMesh() {
		std::vector<Mesh::Vertex> vertices = {
			
			{{0.f, 0.5f, 1.f}, {1.f, 1.f, 1.f, 1.f}},  
			{{-0.25f, -0.5f, 1.f}, {1.f, 1.f, 1.f, 1.f}},
			{{0.25f, -0.5f, 1.f}, {1.f, 1.f, 1.f, 1.f}},

			
			{{-0.05f, -0.5f, 0.f}, {1.f, 1.f, 1.f, 1.f}},
			{{0.05f, -0.5f, 0.f}, {1.f, 1.f, 1.f, 1.f}},
			{{0.05f, 0.5f, 0.f}, {1.f, 1.f, 1.f, 1.f}},
			{{-0.05f, 0.5f, 0.f}, {1.f, 1.f, 1.f, 1.f}},
		};

		std::vector<unsigned int> indices = {
			0,1,2,        // tip
			3,4,5, 5,6,3  // line
		};

		mesh = std::make_unique<Mesh>(vertices, indices);
	}

	void draw(const glm::mat4& view, const glm::mat4& projection) override {
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotationYaw), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(2.f));
		shader->use();
		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		mesh->draw();
	}
};

class AABBorder : public Drawable {
public:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 position = glm::vec3(0.f);

	AABBorder(std::shared_ptr<Shader> s, const glm::vec3& minV, const glm::vec3& maxV)
		: shader(s), min(minV), max(maxV)
	{
		setupMesh();
	}

	void setBounds(const glm::vec3& minV, const glm::vec3& maxV) {
		min = minV;
		max = maxV;
		setupMesh(); // redo mesh with new corners
	}

	void setPosition(const glm::vec3& pos) { position = pos; }

	void draw(const glm::mat4& view, const glm::mat4& projection) override {
		//glm::mat4 model = glm::translate(glm::mat4(1.f), position);
		glm::mat4 model = glm::mat4(1.f);
		//model = glm::scale(model, glm::vec3(2.f));

		shader->use();
		shader->setMat4("model", model);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		mesh->draw(); // uses the Mesh::draw() function
	}

private:
	void setupMesh() {
		// 8 corners
		glm::vec3 corners[8] = {
			{min.x, min.y, min.z},
			{max.x, min.y, min.z},
			{min.x, max.y, min.z},
			{max.x, max.y, min.z},
			{min.x, min.y, max.z},
			{max.x, min.y, max.z},
			{min.x, max.y, max.z},
			{max.x, max.y, max.z}
		};

		// 12 edges 
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int edgeIndices[24] = {
			0,1, 1,3, 3,2, 2,0, // bottom
			4,5, 5,7, 7,6, 6,4, // top
			0,4, 1,5, 2,6, 3,7  // sides
		};

		for (int i = 0; i < 24; ++i) {
			vertices.push_back({ corners[edgeIndices[i]], {1.f, 0.f, 0.f, 1.f} }); // red wireframe
			indices.push_back(i);
		}

		mesh = std::make_unique<Mesh>(vertices, indices); 
	}
};

class deathScreen : public Drawable {

	std::unique_ptr<Mesh> mesh; //points that can be drawn
	std::shared_ptr<Shader> shader;
	float Alpha = 0.f; //how transparent it is
	bool finished = false; //is alpha > 1

public:
	deathScreen(std::shared_ptr<Shader> shader) :
		shader(shader)
	{
		setupMesh();
	}

	void setupMesh() {

		std::vector<Mesh::Vertex> verticies = {
			{{1.f,1.f,0.f}, {0.f,0.f,0.f,1.f}}, //flat plane across teh whole screen with the colour being black, and the alpha being how transparent it is.
			{{1.f,-1.f,0.f}, {0.f,0.f,0.f,1.f}}, //alpha changes every time, meaning that it should slowly get darker
			{{-1.f,-1.f,0.f}, {0.f,0.f,0.f,1.f}},
			{{-1.f,1.f,0.f}, {0.f,0.f,0.f,1.f}}
		};

		std::vector<unsigned int> indicies = {  //second half of the flat plane
			0, 1, 3,   
			1, 2, 3    
		};

		mesh = std::make_unique<Mesh>(verticies, indicies); //create the mesh once per everytime its called
	}
	
	void update() { 
		Alpha += 0.0005f; //increase the alpha
		
		if (Alpha >= 1) { //check if alpha is bigger than 1
			Alpha = 1.f;
			finished = true; //if it is the animation has finished
		}
	}

	bool checkFinished() const { return finished; } 

	void draw(const glm::mat4& projection, const glm::mat4& view) override { //draws it

		glDisable(GL_DEPTH_TEST);	

		shader->use();
		shader->setFloat("uAlpha", Alpha);
		shader->setMat4("model", glm::mat4(1.f));
		shader->setMat4("view", glm::mat4(1.f));
		shader->setMat4("projection", glm::mat4(1.f));

		mesh->draw();

		glEnable(GL_DEPTH_TEST);
	}
};

