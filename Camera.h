#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GLFWwindow;

class Camera {
public:
    //attributes so the camera can work out where we are
    glm::vec3 Position;
    glm::vec3 Front = glm::vec3(0.f,0.f,-1.f);
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // airplane angles 
    float Yaw; //tilt left right 
    float Pitch; //up down

    // options
    float MovementSpeed = 2.5f;
    float Zoom = 45.f;

    //mouse 
    float lastX = 400, lastY = 300;
    bool firstMouse = true;

    //void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    Camera(glm::vec3 startPos = glm::vec3(0.f, 0.f, 3.f), 
        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = -90.f, float pitch = 0.f);
    
    glm::mat4 getViewMatrix() const;

private:
    void updateCameraVectors();

};
