#include "Camera.h"
#include <GLFW/glfw3.h>


Camera::Camera(glm::vec3 startPos, glm::vec3 up, float yaw, float pitch) 
	: Position(startPos) , WorldUp(up), Yaw(yaw), Pitch(pitch)
{
	updateCameraVectors(); //create the vector
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
	//the view matrix is what the camera can see
}

void Camera::updateCameraVectors() {

	glm::vec3 front; //front = where its looking
	//do some maths
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)); 
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	//normalise it so the numbers are between 0 and 1
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
