#include "Mesh.h"
#include "Player.h"
#include <limits> 

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int> inds)
	: vertices(verts), indices(inds)
{
	setupMesh();
	calculateLocalAABB();
}


void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO); //creates the VAO, VBO, EBO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); //actives the VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //sets the target memory in the GPU where the data is going to be stored
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); //uploads the data to the GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //sets the target memory for verticies 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); //uploads the index vector
											//finds the total size of indicies (bits wise), and then returns a pointer to the start of the data
											 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colour));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); //deactivates the VAO
}

void Mesh::draw() const {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::calculateLocalAABB() {

	glm::vec3 min(FLT_MAX); //set the biggest values posible
	glm::vec3 max(-FLT_MAX); //sets the smallest values posible

	for (const auto& verts : vertices) {

		//finds the smallest point in the mesh
		min.x = std::min(min.x, verts.Position.x);
		min.y = std::min(min.y, verts.Position.y);
		min.z = std::min(min.z, verts.Position.z);

		//finds the biggest point in the mesh
		max.x = std::max(max.x, verts.Position.x);
		max.y = std::max(max.y, verts.Position.y);
		max.z = std::max(max.z, verts.Position.z);
	}

	localAABB = AABB(min, max); //constructs the local AABB
}