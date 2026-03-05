#include "WorldObjects.h"
#include <GLFW/glfw3.h>
#include "AssimpMeshBuilder.h"
#include "Player.h"


/// floor
Floor::Floor(float size, std::shared_ptr<Shader> parameterShader)
    
{
    if (!parameterShader) {
        throw std::runtime_error("shader didnt get passed to the floor");
    }
    
    shader = parameterShader;


    std::vector<Mesh::Vertex> vertices = {
        {{-size, -0.1f, -size}, {0.f, 1.f, 0.f, 1.f}},
        {{ size, -0.1f, -size}, {0.f, 1.f, 0.f, 1.f}},
        {{ size, -0.1f,  size}, {0.f, 1.f, 0.f, 1.f}},
        {{-size, -0.1f,  size}, {0.f, 1.f, 0.f, 1.f}}
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    mesh = std::make_unique<Mesh>(vertices, indices);

}

void Floor::draw(const glm::mat4& view, const glm::mat4& projection) {

    glm::mat4 model(1.f);
    shader->use();
    shader->setBool("angry", false);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    mesh->draw();
}


/// cube
Cube::Cube(std::shared_ptr<Shader> s) {

    if (!s)
        throw std::runtime_error("player shader not loaded");

    shader = s;

    std::vector<Mesh::Vertex> vertices = {
        // positions            // colors
        {{-0.5f, 0.1f, -0.5f}, {1, 0, 0, 1}}, // bottom 0
        {{ 0.5f, 0.1f, -0.5f}, {0, 1, 0, 1}}, // bottom 1
        {{ 0.5f, 1.0f, -0.5f}, {0, 0, 1, 1}}, // top 2
        {{-0.5f, 1.0f, -0.5f}, {1, 1, 0, 1}}, // top 3
        {{-0.5f, 0.1f,  0.5f}, {1, 0, 0, 1}}, // bottom 4
        {{ 0.5f, 0.1f,  0.5f }, {0, 1, 0, 1}}, // bottom 5
        {{ 0.5f, 1.0f,  0.5f}, {1, 1, 1, 0}}, // top 6
        {{-0.5f, 1.0f,  0.5f}, {0, 0, 0, 1}} // top 7
    };

    // every point should appear 4 times 
    std::vector<unsigned int> indices = {
        // back face
        0, 1, 2,
        2, 3, 0,
        // front face
        4, 5, 6,
        6, 7, 4,
        // left face
        4, 0, 3,
        3, 7, 4,
        // right face
        1, 5, 6,
        6, 2, 1,
        // bottom face
        4, 5, 1,
        1, 0, 4,
        // top face
        3, 2, 6,
        6, 7, 3
    };

    mesh = std::make_unique<Mesh>(vertices, indices);
}

void Cube::draw(const glm::mat4& view, const glm::mat4& projection) {

    glm::mat4 model = glm::rotate(glm::mat4(1.f), (float)glfwGetTime(), glm::vec3(0.5f, 1.f, 0.f)); //rotates anything below it 

    shader->use();
    shader->setBool("angry", false);
    shader->setMat4("model", model); //if i put floor.draw below this it rotates
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    mesh->draw();

}

///coin 

Coin::Coin(std::shared_ptr<Shader> s, glm::vec3 pos) {

    shader = s;
    position = pos;
    Coin::setupMesh();
}

void Coin::draw(const glm::mat4& view, const glm::mat4& projection) {

    if (collected) { return; } //coin already picked up

    model = glm::mat4(1.f);

    model = glm::translate(model, position); //moving the coin to the position

    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.f, 0.f)); //spinning the coin

    shader->use(); 
    shader->setBool("angry", false);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    mesh->draw();
}

void Coin::setupMesh() {
    mesh = loadModel("Coin.glb");
}

void Coin::update() {

    model = glm::translate(glm::mat4(1.f), position);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.f, 0.f));

    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));

}

///generic world objects
void WorldObjects::draw(const glm::mat4& view, const glm::mat4& projection) {
    model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(actualScale));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    shader->use();
    shader->setBool("angry", false);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);
    mesh->draw();
}

void WorldObjects::setGlobalAABB() {

    mesh->getLocalAABB().min = mesh->getLocalAABB().min * scaleOfAABB;
    mesh->getLocalAABB().max = mesh->getLocalAABB().max * scaleOfAABB;


    model = glm::translate(glm::mat4(1.f), position);
    model = glm::scale(model, glm::vec3(actualScale));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));
}



void redArrow::update(Player& player) {
    position = player.getPlayerPosition() + glm::vec3(0.0f, 1.0f, 0.0f); // slightly above player
    direction = glm::normalize(player.getPlayerfront()); // point in player’s facing direction

}

void greenArrow::update(const glm::vec3& playerPos, const glm::vec3& cameraFront) {
    // Position slightly above the player
    position = playerPos + glm::vec3(0.f, 1.f, 0.f);

    // Get yaw angle from cameraFront
    if (glm::length(cameraFront) > 0.0001f) {
        // Project onto XZ plane
        glm::vec3 flatFront = glm::normalize(glm::vec3(cameraFront.x, 0.f, cameraFront.z));
        rotationYaw = glm::degrees(atan2(flatFront.x, flatFront.z));
    }
}

