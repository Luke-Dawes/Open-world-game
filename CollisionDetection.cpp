#include "CollisionDetection.h"
#include <iostream>
#include <algorithm>
#include <cmath>

bool AABB::checkContact(const AABB& other) {
	/*
	* checks contact between two AABBs.
	* it does it by comparing the min with the other max, and the max with the other min
	* 
	* this is beacuse all 3 axis have to overlapping for there to be a collision, and this
	* checks if they are overlapping, with both the max and min.
	*/

	return (min.x <= other.max.x && max.x >= other.min.x) && //compares the x
		   (min.y <= other.max.y && max.y >= other.min.y) && //compares the y
		   (min.z <= other.max.z && max.z >= other.min.z); //compares the z
}


glm::vec3 AABB::calculateMTV(const AABB& other) {
	//calculates the shortest vector which could stop the collision

	//work out the centers from the AABB's. - could pass in pos of the objects
	//but this will also work
	glm::vec3 center = (min + max) * 0.5f;
	glm::vec3 otherCenter = (other.min + other.max) * 0.5f;

	//find the distance between the objects
	glm::vec3 distance = otherCenter - center;

	//calculate the half distances.
	//half distance is the max distance from the center of the box to the 
	//furthest away point, which then allows us to calculate the smallest overlap
	glm::vec3 halfSizeA = (max - min) * 0.5f;
	glm::vec3 otherHalfSize = (other.max - other.min) * 0.5f;

	//find how bigger overlap there is by adding the halfsize to the other half size, and then
	//minusing the distnace between the two axis. i think this gives us the distance that
	//overlaps on each axis. 
	float overlapX = halfSizeA.x + otherHalfSize.x - std::abs(distance.x);
	float overlapY = halfSizeA.y + otherHalfSize.y - std::abs(distance.y);
	float overlapZ = halfSizeA.z + otherHalfSize.z - std::abs(distance.z);

	//if distance is smaller than 0, the otherAABB is on the left (negative) side of the AABB
	//this means that we need to push it in a negative direction. if its positive its on the right
	//(or positive) side of this AABB

	if (overlapX < overlapY && overlapX < overlapZ) {
		//if x is the smallest overlap
		if (distance.x < 0) {
			return glm::vec3(overlapX, 0.f, 0.f);
		}
		else 
		{
			return glm::vec3(-overlapX, 0.f, 0.f);
		}
	}
	else if (overlapY < overlapZ) {
		//if y is the smallest overlap
		if (distance.y < 0) {
			return glm::vec3(0.f, overlapY, 0.f);
		}
		else
		{
			return glm::vec3(0.f, -overlapY, 0.f);
		}
	}
	else {
		//if z is the smallest overlap
		if (distance.z < 0) {
			return glm::vec3(0.f, 0.f, overlapZ);
		}
		else
		{
			return glm::vec3(0.f, 0.f, -overlapZ);
		}

	}
}



AABB TransformAABB(const AABB& localAABB, const glm::mat4& model) {

	glm::vec3 minWorld(FLT_MAX); //sets it to the max value
	glm::vec3 maxWorld(-FLT_MAX); //sets it to the min value

	auto& minLocal = localAABB.min; //easier to write, and as passing by refrence its no slower
	auto& maxLocal = localAABB.max;

	/*
	* gets an array which is size 8, of the 8 points of a rectangle. (AABB is just a box)
	* this is made by the local AABB, beacuse i need to translate each corner.
	*/
	glm::vec3 corners[8] =
	{
		{minLocal.x, minLocal.y, minLocal.z},
		{maxLocal.x, minLocal.y, minLocal.z},
		{minLocal.x, maxLocal.y, minLocal.z},
		{maxLocal.x, maxLocal.y, minLocal.z},
		{minLocal.x, minLocal.y, maxLocal.z},
		{maxLocal.x, minLocal.y, maxLocal.z},
		{minLocal.x, maxLocal.y, maxLocal.z},
		{maxLocal.x, maxLocal.y, maxLocal.z}
	};

	for (int i = 0; i < 8; i++) { //loop through all 8 points

		//translate them into the world space
		//model is a mat4, meaning that it cant be multipled by a vec3.
		//instead, i just add a pointless value at the end which does nothing
		//and it instantly gets cut off beacuse we turn it into a vec3 after the multiplition
		glm::vec3 p = glm::vec3(model * glm::vec4(corners[i], 1.0f));

		//glm has a min and max function which works with vec3,
		//finds the min and max points in the AABB, which can then be used for collision detection
		minWorld = glm::min(minWorld, p);
		maxWorld = glm::max(maxWorld, p);
	}

	//returns the values which can be used in a constructor, beacuse each object needs to own its local AABB
	//AND its AABB in world space
	return { minWorld, maxWorld };

}
