#include "Camera.h"
#include <GLFW/glfw3.h>


Camera::Camera(glm::vec3 startPos, glm::vec3 up, float yaw, float pitch) 
	: Position(startPos) , WorldUp(up), Yaw(yaw), Pitch(pitch)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors() {

	glm::vec3 front;

	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}