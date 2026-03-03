#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "CollisionDetection.h"

class Mesh {

public:
	struct Vertex //struct to hold the data together but split it into catagorys
	{
		glm::vec3 Position; //a vector to hold 3 floats (maybe an array), and is to do with the position of the traingle
		glm::vec4 Colour; //a vector to hold 4 floats (which is maybe an array), and is to do with colour
	};

	std::vector<Vertex> vertices; //holds the verticies (the corners of the triangles)
	std::vector<unsigned int> indices; //holds how many triangles and what vertices to use to make a triangle

	Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int> inds);

	void draw() const;

	AABB& getLocalAABB() {return localAABB;}

	~Mesh() { //if mesh is deleted, delete the GPU data to do with mesh
		if (VBO) glDeleteBuffers(1, &VBO);
		if (EBO) glDeleteBuffers(1, &EBO);
		if (VAO) glDeleteVertexArrays(1, &VAO);
	}

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();

	AABB localAABB;

	void calculateLocalAABB();

};
