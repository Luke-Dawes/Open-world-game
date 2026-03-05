#include "Player.h"
#include "Main.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::shared_ptr<Mesh> PlayerProjectile::mesh = nullptr;

Player::Player(std::string pName, std::shared_ptr<Shader> s, std::shared_ptr<Inventory> I, std::shared_ptr<Settings> S) {



	if (!s)
		throw std::runtime_error("player shader not loaded");

	shader = s;

    name = pName;
    inventory = I;
    settings = S;
    Player::setupMesh();
}

void Player::draw(const glm::mat4& view, const glm::mat4& projection) {

    //glm::mat4 model = glm::rotate(glm::mat4(1.f), (float)glfwGetTime(), glm::vec3(0.5f, 1.f, 0.f)); //rotates anything below it 

    model = glm::mat4(1.f);

    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(yaw-180), glm::vec3(0, -1, 0)); //rotate the player 

    


    shader->use();
    shader->setBool("angry", false);
    shader->setMat4("model", model); //if i put floor.draw below this it rotates
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    mesh->draw();

}

void Player::setupMesh() {

    mesh = loadModel("Adventurer.glb"); ///put in the path to the model, 

    if (mesh->getLocalAABB().max != glm::vec3(1.f)) { //check to see if its calculated yet
        mesh->getLocalAABB().max.y = mesh->getLocalAABB().max.y * 1.4f; //scale the max.y to a higher height
    }
}

void Player::handleInput(GLFWwindow* window, float deltaTime) {

    velocity.x = 0.f;
    velocity.z = 0.f;
    
    //game state options
    if (glfwGetKey(window, settings->getPause()) == GLFW_PRESS) {
        state = Gamestate::PausedMenu;
    }
    if (glfwGetKey(window, settings->getInventory()) == GLFW_PRESS) {
        state = Gamestate::Inventory;
    }

    interact = false;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        interact = true;
    }

    //attacking
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {

        if (glfwGetTime() > lastAttack) {
            playerproj.emplace_back(std::make_shared<PlayerProjectile>(5, playerFront, position, shader));

            setLastAttack(glfwGetTime() + attackDelay);

            std::cout << "spawn";
        }
    }

    //movement
    if (glfwGetKey(window, settings->getForwards()) == GLFW_PRESS) {
        velocity.z += speed; 
    }
    if (glfwGetKey(window, settings->getBackwards()) == GLFW_PRESS) {
        velocity.z -= speed;
    }
    if (glfwGetKey(window, settings->getLeft()) == GLFW_PRESS) {
        //velocity.x -= speed; //makes the x velocity lower (goes towards minus x)
        yaw -= rotationspeed * deltaTime; //increases the yaw
    }
    if (glfwGetKey(window, settings->getRight()) == GLFW_PRESS) {
        //velocity.x += speed;
        yaw += rotationspeed * deltaTime; //decreases the yaw
    }

    //jump
    if (glfwGetKey(window, settings->getJump()) == GLFW_PRESS && onGround) {
        velocity.y = jumpHeight;
        onGround = false;
    }

    //normalising movement caluclation
    glm::vec2 movement = glm::vec2(velocity.x, velocity.z);

    float length = glm::length(movement);

    if (length > 0.001f) { //normalising movement
        movement = (movement/length) * speed; 
        velocity.x = movement.x;
        velocity.z = movement.y;
    }

    playerFront.x = sin(glm::radians(yaw)); //gets the correct forward angle for x
    playerFront.y = 0; //doesnt need an angle 
    playerFront.z = -cos(glm::radians(yaw)); //gets the correct height forwad angle
    playerFront = glm::normalize(playerFront); //normalise it 


    handlePhysics(deltaTime);
}

void Player::handlePhysics(float deltaTime) {

    if (!onGround) {
        velocity.y -= gravity * deltaTime;
    }

    glm::vec3 playerRight = glm::normalize(glm::cross(playerFront, glm::vec3(0, 1, 0))); //gets the movement left right
    glm::vec3 moveDir = glm::vec3(0.f);
    moveDir += velocity.z * playerFront;  //sets the forward movement set to the correct direction
    moveDir += velocity.x * playerRight;  //sets the left right movement in the correct direction
    moveDir.y = velocity.y;


    position += moveDir * deltaTime; //correctly moves the player

    if (position.y <= 0) {
        position.y = 0;
        velocity.y = 0;
        onGround = true;
    }

}

void Player::pickUpCoins(std::vector<std::shared_ptr<Coin>>& coins) {
    for (auto& coin : coins) { //loop through coins
        coin->update(); //update their global AABB's
        if (!coin->isCollected()) { //if coin is not collected
             
            if (globalAABB.checkContact(coin->getGlobalAABB())) { //check if they touch
                coin->collect(); //if they do collect it 
                money += coin->getValue(); //and add money
                std::cout << "\n" << money << "\n"; //print for debug

            //float distance = glm::length(position - coin->getPosition());
            //if (distance < radius + coin->getRadius()) {
            //    coin->collect();
            //    money += coin->getValue();
            //    std::cout << "\n" << money << "\n";
            }
        }
    }
}

void Player::takeDamage(int value) { 

    if (glfwGetTime() - lastDamageTime < 0.2) return;
    //if the time is less than one second, you wont take damage

    lastDamageTime = glfwGetTime(); //set the last damage time 

    health = std::max(0, health - value); //find the bigger value

    if (health == 0) {
        
        alive = false;
        //player is dead - change the screen to a death menu
    }
}

void Player::update(GLFWwindow* window, float dt, std::vector<std::shared_ptr<Coin>> coins) {

    handleInput(window, dt); //run movement 
    pickUpCoins(coins);

    //creates the model which the AABB will be scaled with
    model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0)); //rotate the model 
    model = glm::scale(model, glm::vec3(0.7f));

    //scales the AABB
    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));

    if (!playerproj.empty()) {

        float time = glfwGetTime();

        for (int i = 0; i < playerproj.size(); i++) {

            playerproj[i]->update(window, dt, time);
            
        }
    }
}

void Player::move(glm::vec3 pos) {
    //move the player
    position += pos;

    //remakes the model matrix
    model = glm::translate(glm::mat4(1.f), position);
    model = rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.7f));

    //remakes the AABB for the player
    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));
}

void PlayerProjectile::setupMesh() {
    if (!mesh) { //if mesh == nullptr
        mesh = loadModel("Star.glb");
        
    }
}

void PlayerProjectile::update(GLFWwindow* window, float dt, float time) {

    if (expiraryTime < time) {
        //this->~PlayerProjectile(); //calls the destructor 
        isAlive = false;
        return;
    }

    position += velocity * dt;

    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.008f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.f, 0.f, 0.f));

    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));
}

void PlayerProjectile::draw(const glm::mat4& view, const glm::mat4& projection) {


    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.008f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.f, 0.f, 0.f)); 
    shader->use();
    shader->setBool("angry", false);
    shader->setMat4("model", model); //if i put floor.draw below this it rotates
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    mesh->draw();
}