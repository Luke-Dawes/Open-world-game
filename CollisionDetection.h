#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct AABB { //for rectangles
	glm::vec3 min;
	glm::vec3 max;

	AABB() = default;

	AABB(glm::vec3 min, glm::vec3 max) //contruct with a min and max
		: min(min), max(max)
	{}

	bool checkContact(const AABB& other);

	glm::vec3 calculateMTV(const AABB& other);

	//each mesh will have their own local AABB, and then i just need to 
	//add the AABB to the position in space to work with it. 

};

AABB TransformAABB(const AABB& localAABB, const glm::mat4& model);