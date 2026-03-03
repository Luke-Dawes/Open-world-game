#include "NPC.h"
#include "AssimpMeshBuilder.h"
#include "Player.h"
#include <fstream>
#include <sstream>
#include <iostream>


NPC::NPC(const std::string& name, std::shared_ptr<Shader> shader, glm::vec3 position, std::shared_ptr<Mesh> mesh)
	: name(name), shader(shader), position(position), mesh(mesh) 
{
}


void NPC::draw(const glm::mat4& view, const glm::mat4& projection) {

    glm::mat4 model = glm::mat4(1.f); //makes a new mat 4 which is all 1's

    model = glm::translate(model, position); //moves the model to the new position

    model = glm::scale(model, glm::vec3(0.5f)); //swapping translate and scale breaks it (maybe it scales the translation?)

    shader->use(); //select which VAO to use

    //selects if the AI is angry or not, leading to the different colours
    bool angry = (state == NPCState::ANGRY) ? true : false;
    shader->setBool("angry", angry);

    shader->setMat4("model", model); //sets the uniform model to the value of model
    shader->setMat4("view", view); //sets the uniform view to the view
    shader->setMat4("projection", projection); //sets the uniform projection to the value of projection

    mesh->draw(); //draws it

}

void NPC::update(float deltaTime, Player& player) {

    glm::vec3 targetPos = player.getPlayerPosition(); //find the target pos (the human position)

    glm::vec3 direction = targetPos - position; //gets the direction / if ai is on top of human it would be 0

    float length = glm::length(direction); //finds the distance between AI and player

    //std::cout << length; debugging

    if (length > rangeRadius) { //if the distance is bigger than the range, lose intrest
        state = NPCState::PASSIVE;
    }
    else { //if the distace is inside the range, gain intrest/keep intrest
        state = NPCState::ANGRY;
    }

    //making ai follow the player if in an angry state. 
    if (state == NPCState::ANGRY) {

        //if (length < 0.5f) { //if the player is in range
        //    player.takeDamage(damage);
        //}

        if (length > 0.001f) { //length finds the hypotenuse between the values, and if thats bigger than 0)
            auto normalisedDirection = glm::normalize(direction); //direction get normalised
            position += normalisedDirection * speed * deltaTime; //this just means go in the correct direction at the speed * delta time
            
        } 
        //std::cout << position.x << " " << position.y << " " << position.z << "\n";
    }
    //if in a passive state.
    else if (state == NPCState::PASSIVE) {
        
        glm::vec3 passiveDirection = currentTarget - position; //finds the distance to the target

        if (glm::length(passiveDirection) > 0.001f) { //finds the distance to the location

            position += glm::normalize(passiveDirection) * speed * deltaTime; 
            //moves towards the current target

            
        }
        else {
            float randomNumber = (int)glfwGetTime() % randomPositions.size(); //get a random number which is below the size of random positions

            currentTarget = randomPositions[randomNumber]; //select a random position in the vector.
        }
    }

    //glm::mat4 model = glm::mat4(1.f); //makes a new mat 4 which is all 1's
    //model = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));
    model = glm::mat4(1.f);
    model = glm::translate(model, position); //moves the model to the new position

    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));

}

void NPC::move(glm::vec3 pos) {
    //move the NPC
    position += pos;

    //move the model matrix to the new spot
    model = glm::translate(glm::mat4(1.f), position);

    //remake the AABB in the new spot
    globalAABB = AABB(TransformAABB(mesh->getLocalAABB(), model));
}


void FriendlyNPC::setupDialogue() 
{
    speech = {
        {
            "hello traveler",
            [](std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {return true; }
        },
        {
            "there are wolves attacking my sheep, please help",
            [](std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {return true; }
        },
        {
            "You can shoot them using the z key",
            [](std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {return true; }
        },
        {
            "there are still some wolves left which could kill my sheep",
            [](std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {return NPCVec.size() != 0; }
        },
        {
            "You defended my sheep, thank you",
            [](std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {return sheepVec.size() == 3 && NPCVec.size() == 0; }
        },
        {
            "You killed one of my sheep, try again",
            [](std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {return sheepVec.size() != 3 && NPCVec.size() == 0; }
        },
    };

}

std::string FriendlyNPC::speak(std::vector<std::shared_ptr<Sheep> >& sheepVec, std::vector<std::shared_ptr<NPC> >& NPCVec) {

    //this will display one line of text everytime its ran,
    //and it will skip nodes which return false.

    while (npcDialogueIndex < speech.size()) { //checks range error
        auto& node = speech[npcDialogueIndex]; //gets the node 

        if (node.condition(sheepVec, NPCVec)) { //if the bool is true 

           return node.text; 
        }
        npcDialogueIndex++; //increments it by one
    }
    while (true) { //force there to always be something
        npcDialogueIndex -= 1;
        auto& node = speech[npcDialogueIndex];

        if (node.condition(sheepVec, NPCVec)) {
            return node.text;
        }
    }
}