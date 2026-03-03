#include "GameManager.h"
#include "ImGuiMenus.h"
#include "SaveLoad.h"
#include <memory>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

bool Manager::initGame(std::shared_ptr<Camera> preCamera, std::string name) {

	auto sharedShader = std::make_shared<Shader>("C:/courseWork/courseWork3D/Shaders/Player.vert.txt", //=============== WILL CAUSE ERRORS WHEN TRY TO PLAY ON OTHER MACHINES
        "C:/courseWork/courseWork3D/Shaders/Player.frag.txt");

    inventory = std::make_shared<Inventory>(); //create the inventory 

    settings = std::make_shared<Settings>(); //create the settings 

    player = std::make_unique<Player>(name, sharedShader, inventory, settings); //pass in settings and inventory to the player
    PlayerProjectile::setupMesh(); //uses it for the static function

    cube = std::make_unique<Cube>(sharedShader);

    DeathScreen = std::make_shared<deathScreen>(std::make_shared<Shader>("C:/courseWork/courseWork3D/Shaders/DeathScreen.vert.txt", //=============== WILL CAUSE ERRORS WHEN TRY TO PLAY ON OTHER MACHINES
        "C:/courseWork/courseWork3D/Shaders/DeathScreen.frag.txt"));

    if (preCamera != nullptr) {
        camera = preCamera;
    }
    else {  
        camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f));
    }

    floor = std::make_unique<Floor>(50.f, sharedShader);

    friendlyNPC = std::make_shared<FriendlyNPC>("NPC1", sharedShader, glm::vec3(5.f,0.f,5.f), loadModel("C:/courseWork/courseWork3D/x64/Debug/Adventurer.glb"));

    NPCVec.push_back(std::make_shared<Wolf>("wolf", sharedShader, glm::vec3(-20.f, 0, -18.f), 10, loadModel("C:/courseWork/courseWork3D/x64/Debug/Wolf.glb")));
    NPCVec.push_back(std::make_shared<Wolf>("wolf", sharedShader, glm::vec3(-23.f, 0, -15.f), 10, loadModel("C:/courseWork/courseWork3D/x64/Debug/Wolf.glb")));
    NPCVec.push_back(std::make_shared<Wolf>("wolf", sharedShader, glm::vec3(-25.f, 0, -13.f), 10, loadModel("C:/courseWork/courseWork3D/x64/Debug/Wolf.glb")));

    coinsVec.push_back(std::make_shared<Coin>(sharedShader, glm::vec3(-45.f, 1.f, -45.f)));
    coinsVec.push_back(std::make_shared<Coin>(sharedShader, glm::vec3(-16.f, 1.f, -34.f)));
    coinsVec.push_back(std::make_shared<Coin>(sharedShader, glm::vec3(7.f, 1.f, -26.f)));
    coinsVec.push_back(std::make_shared<Coin>(sharedShader, glm::vec3(-32.f, 1.f, -11.f)));
    coinsVec.push_back(std::make_shared<Coin>(sharedShader, glm::vec3(-28.f, 1.f, -26.f)));

    sheepVec.push_back(std::make_shared<Sheep>("Sheep", sharedShader, glm::vec3(-40.f, 0.f, -36.f), loadModel("C:/courseWork/courseWork3D/x64/Debug/Sheep.glb")));
    sheepVec.push_back(std::make_shared<Sheep>("Sheep", sharedShader, glm::vec3(-39.f, 0.f, -35.5f), loadModel("C:/courseWork/courseWork3D/x64/Debug/Sheep.glb")));
    sheepVec.push_back(std::make_shared<Sheep>("Sheep", sharedShader, glm::vec3(-40.f, 0.f, -33.f), loadModel("C:/courseWork/courseWork3D/x64/Debug/Sheep.glb")));

    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Cabin.glb", sharedShader, glm::vec3( 8.f, 3.6f, -20.f), 0.85f, 0.009f, 90)); //cabin is here
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-40.5f, 0.1f, -40.f), 1.f, 5.f, 45.f)); //first fense - curved back left
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-42.f, 0.1f, -36.f), 1.f, 5.f, 90.f)); //second fense - parralel to house back left + 1
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-42.f, 0.1f, -32.f), 1.f, 5.f, 90.f)); //third fense - parrallel to house back left + 2
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-42.f, 0.1f, -28.f), 1.f, 5.f, 90.f)); //fourth fense - parrallel to house back left + 3
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-42.f, 0.1f, -24.f), 1.f, 5.f, 90.f)); //fith fense - parrallel to house back left + 4
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-42.f, 0.1f, -20.f), 1.f, 5.f, 90.f)); //sixth fense - parrallel to house back left + 5
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-41.f, 0.1f, -17.f), 1.f, 5.f, 315.f)); //seventh fense - coming towards the player front left
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-38.7f, 0.1f, -14.5f), 1.f, 5.f, 315.f)); //eights fense - cming towards the player front left + 1
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-37.f, 0.1f, -40.f), 1.f, 5.f, 0.f)); //ninth fense - straight back left going towards the right 
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-33.f, 0.1f, -40.f), 1.f, 5.f, 0.f)); //tenth fense - straight back left going towards the right + 1
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-29.f, 0.1f, -40.f), 1.f, 5.f, 0.f)); //eleventh fense - straight back left going towards the right + 2
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-25.f, 0.1f, -40.f), 1.f, 5.f, 0.f)); //twelth fense - straight back left going towards the right + 3
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-22.f, 0.1f, -38.f), 1.f, 5.f, 315.f)); //thirteen fense - going towards house at angle - rightside
    worldObjectsVec.push_back(std::make_shared<WorldObjects>("C:/courseWork/courseWork3D/x64/Debug/Fence.glb", sharedShader, glm::vec3(-20.f, 0.1f, -36.f), 1.f, 5.f, 315.f)); //fourteen fense - going towards house at angle + 1 - rightside

#ifdef _DEBUG
    NPCWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, NPCVec[0]->getGlobalAABB().min, NPCVec[0]->getGlobalAABB().max)); ///debug
    NPCWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, NPCVec[1]->getGlobalAABB().min, NPCVec[1]->getGlobalAABB().max)); ///debug
    NPCWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, NPCVec[2]->getGlobalAABB().min, NPCVec[2]->getGlobalAABB().max)); ///debug
    sheepWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, sheepVec[0]->getGlobalAABB().min, sheepVec[0]->getGlobalAABB().max)); ///debug
    sheepWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, sheepVec[1]->getGlobalAABB().min, sheepVec[1]->getGlobalAABB().max)); ///debug
    sheepWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, sheepVec[2]->getGlobalAABB().min, sheepVec[2]->getGlobalAABB().max)); ///debug
    playerFrame = std::make_shared<AABBorder>(sharedShader, player->getGlobalAABB().min, player->getGlobalAABB().max);
    coinWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, coinsVec[0]->getGlobalAABB().min, coinsVec[0]->getGlobalAABB().max));
    coinWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, coinsVec[1]->getGlobalAABB().min, coinsVec[1]->getGlobalAABB().max));
    coinWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, coinsVec[2]->getGlobalAABB().min, coinsVec[2]->getGlobalAABB().max));
    coinWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, coinsVec[3]->getGlobalAABB().min, coinsVec[3]->getGlobalAABB().max));
    coinWireFrameVec.push_back(std::make_shared<AABBorder>(sharedShader, coinsVec[4]->getGlobalAABB().min, coinsVec[4]->getGlobalAABB().max));

    houseFrame = std::make_shared<AABBorder>(sharedShader, worldObjectsVec[0]->getGlobalAABB().min, worldObjectsVec[0]->getGlobalAABB().max);
    
#endif

    if (!player || !cube || !camera || !floor || !inventory || !settings) return false;

    return true;
}


void Manager::mainLoop(GLFWwindow* window) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int frames = 0;
    double prevTime = glfwGetTime();
    double DeltaPrevTime = prevTime;
    int fps = 0;
    float lastSave = 0;

    //enables 3d
    glEnable(GL_DEPTH_TEST);

    

    //main loop structure
    while (!glfwWindowShouldClose(window))
    {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //fps
        frames++;

        //delta time
        double DeltaCurrent = glfwGetTime();
        double deltaTime = DeltaCurrent - DeltaPrevTime;
        DeltaPrevTime = DeltaCurrent;

        //Inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            state = Gamestate::Quit;
            
        }

        if (state == Gamestate::Playing || (state == Gamestate::Dead && !DeathScreen->checkFinished())) {

            player->update(window, deltaTime, coinsVec);

#ifdef _DEBUG
            playerFrame->setBounds(player->getGlobalAABB().min, player->getGlobalAABB().max);  ///debugging
#endif // DEBUG
            

            for (auto& npc : NPCVec) {
                npc->update(deltaTime, *player); //update the AI
            }

            //COLLISION DETECTION

            //player on world objects
            for (auto& obj : worldObjectsVec) {
                if (player->getGlobalAABB().checkContact(obj->getGlobalAABB())) {
                    glm::vec3 worldAndHuman = player->getGlobalAABB().calculateMTV(obj->getGlobalAABB());
                    player->move(worldAndHuman);
                }
            }

            //sheep on player =====================
            for (auto& sheep : sheepVec) {
                sheep->update(deltaTime, *player);
                if (player->getGlobalAABB().checkContact(sheep->getGlobalAABB())) {
                    //find the shortest distance
                    glm::vec3 npcAndHuman = player->getGlobalAABB().calculateMTV(sheep->getGlobalAABB());

                    //move both half of the total needed
                    player->move(npcAndHuman * 0.5f);
                    sheep->move(-npcAndHuman * 0.5f);
                }

                //sheep on world objects =======================
                for (auto& obj : worldObjectsVec) {
                    if (sheep->getGlobalAABB().checkContact(obj->getGlobalAABB())) {

                        glm::vec3 sheepAndWorld = sheep->getGlobalAABB().calculateMTV(obj->getGlobalAABB());

                        sheep->move(sheepAndWorld);

                    }
                }
                //sheep on world objects end =================

            } //sheep on player end ======================

            auto& playerAttacks = player->getPlayerProj(); //get the vector of player attacks

            //player attacks ===============================
            for (int i = 0; i < playerAttacks.size(); i++) { //loop through the attacks
                
                //player attacks on NPC =========================================
                for (auto& npc : NPCVec) { //check against all the AI's

                    //if there is contact
                    if (playerAttacks[i]->getGlobalAABB().checkContact(npc->getGlobalAABB())) { 

                        //set the star to dissapear.
                        playerAttacks[i]->isAlive = false;
                        npc->takeDamage(20); //npc to take damage

                    } //player attacks on NPC end ===================================

                    //player attacks on sheep ============================
                    for (auto& sheep : sheepVec) { //loop through the sheep

                        //if contact between attack and sheep
                        if (playerAttacks[i]->getGlobalAABB().checkContact(sheep->getGlobalAABB())) {

                            //make the star get removed and kill the sheep
                            playerAttacks[i]->isAlive = false;
                            sheep->takeDamage(50);
                        }
                    }
                    //player attacks on sheep end ===================================
                }
            }

            //sheep on sheep ================================
            for (int i = 0; i < sheepVec.size(); i++) {
                
                for (int j = i + 1; j < sheepVec.size(); j++) {

                    //loop through sheep vec
                    if (sheepVec[i]->getGlobalAABB().checkContact(sheepVec[j]->getGlobalAABB())) {
                        //if colision find the shortest vector
                        glm::vec3 collisionMTV = sheepVec[i]->getGlobalAABB().calculateMTV(sheepVec[j]->getGlobalAABB());
                        //apply the vector to the diff sheep
                        sheepVec[i]->move(collisionMTV * 0.5f);
                        sheepVec[j]->move(-collisionMTV * 0.5f);
                    }
                }
            }
            //sheep on sheep end ============================

            //sheep on NPC ===================================
            for (int i = 0; i < NPCVec.size(); i++) {

                for (auto& sheep : sheepVec) {
                    if (NPCVec[i]->getGlobalAABB().checkContact(sheep->getGlobalAABB())) { //if contact 
                        //find the shortest distance
                        glm::vec3 npcAndSheepCollisonMTV = NPCVec[i]->getGlobalAABB().calculateMTV(sheep->getGlobalAABB());

                        //move both half of the total needed
                        NPCVec[i]->move(npcAndSheepCollisonMTV * 0.7f);
                        sheep->move(-npcAndSheepCollisonMTV * 0.3f);
                    }
                } //sheep on NPC end ==============================

                //npc on player =================================
                if (NPCVec[i]->getGlobalAABB().checkContact(player->getGlobalAABB())) { //if theres a collision
                    //std::cout << "collision with ai: " << i << "\n"; ///debugging
                    player->takeDamage(NPCVec[i]->getDamage()); //make the player take damage

                    //find the shortest vector which can be used to solve the collision
                    glm::vec3 collisionMTV = NPCVec[i]->getGlobalAABB().calculateMTV(player->getGlobalAABB());

                    //move both the npc and the player equal amounts to fix the collision
                    NPCVec[i]->move(collisionMTV * 0.5f);
                    player->move(-collisionMTV * 0.5f);
                }
                //npc on player end ==============================

                //NPC on world objects =====================
                for (auto& obj : worldObjectsVec) {
                    if (NPCVec[i]->getGlobalAABB().checkContact(obj->getGlobalAABB())) {
                        glm::vec3 worldAndWolf = NPCVec[i]->getGlobalAABB().calculateMTV(obj->getGlobalAABB());
                        NPCVec[i]->move(worldAndWolf);
                    }
                }
                //NPC on world objects end ===============

                //NPC on NPC =====================================
                //i + 1, so that the ai never checks against itself. 
                for (int j = i + 1; j < NPCVec.size(); j++) {

                    //check if the ai collidse with the next ai
                    if (NPCVec[i]->getGlobalAABB().checkContact(NPCVec[j]->getGlobalAABB())) {

                        //get the shortest vector to resolve the collision
                        glm::vec3 collisionMTV = NPCVec[i]->getGlobalAABB().calculateMTV(NPCVec[j]->getGlobalAABB());

                        //move both the ai and the next ai equal amounts to fix the collision
                        NPCVec[i]->move(collisionMTV * 0.5f);
                        NPCVec[j]->move(-collisionMTV * 0.5f);
                    }
                } //npc on npc end ==================================


#ifdef _DEBUG
                    NPCWireFrameVec[i]->setBounds(NPCVec[i]->getGlobalAABB().min, NPCVec[i]->getGlobalAABB().max); ///debugging
#endif // DEBUG
                //sheepWireFrameVec[i]->setBounds(sheepVec[i]->getGlobalAABB().min, sheepVec[i]->getGlobalAABB().max); ///debugging

            }

            //check if player is dead
            if (!player->checkAlive()) state = Gamestate::Dead;

            int i = 0; ///debugging
#ifdef _DEBUG
            for (auto& coin : coinsVec) {
                coinWireFrameVec[i]->setBounds(coin->getGlobalAABB().min, coin->getGlobalAABB().max);
                i++;
            }            
#endif // DEBUG

            //INIT
            glClearColor(0.f, 0.f, 0.8f, 0.1f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            

            int screenWidth, screenHeight;
            glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

            //RE CALCULATE CAMERA
            
            //camera class
            glm::mat4 projection(glm::perspective(glm::radians(45.f), (float)screenWidth / (float)screenHeight, 0.1f, 100.f));


            glm::vec3 playerPos = player->getPlayerPosition();
            float yaw = player->getYaw();

            glm::vec3 cameraOffset = glm::vec3(0.f, 5.f, 10.f); // behind and above
            camera->Position = playerPos - player->getPlayerfront() * cameraOffset.z + glm::vec3(0.f, cameraOffset.y, 0.f); 
            //get the position of the player and multiply it my the z and y offset.
            camera->Front = glm::normalize(playerPos - camera->Position);
            //normalise the front of the camera so it looks in the right direction

            glm::mat4 view = camera->getViewMatrix();

            
            //CLEARING VECTORS
            
            //clear the player proj vector
            //get the vector
            auto& playerProjectiles = player->getPlayerProj();

            //erase the object in the vector if lambda returns true. the bounds are the begin and end
            playerProjectiles.erase(std::remove_if(playerProjectiles.begin(), playerProjectiles.end(),

                //this is basically a for loop which returns true or false, by getting each value in the vector
                //as a refrence and then checking if that refrence is not alive. this is a bool
                [](const std::shared_ptr<PlayerProjectile>& p) {
                    return !p->isAlive;
                }),

                //this end refreses to the erase function, and it tells the erase function where the end of the vec is
                playerProjectiles.end()); 


            NPCVec.erase(std::remove_if(NPCVec.begin(), NPCVec.end(),
                [](const std::shared_ptr<NPC>& npc) {
                    return !npc->isNpcAlive();
                }),
                NPCVec.end());

            sheepVec.erase(std::remove_if(sheepVec.begin(), sheepVec.end(),
                [](const std::shared_ptr<Sheep>& sheep) {
                    return !sheep->isNpcAlive();
                }),
                sheepVec.end());



            //interact with npc
            if (player->getInteract() || friendlyNPC->isTalking) {
                float dist = glm::distance(player->getPlayerPosition(), friendlyNPC->getPosition());
                if (dist < 15.f || friendlyNPC->isTalking) {

                    if (shop) {
                        shop = menu->Shop(window, *this);
                    }
                    else {
                        friendlyNPC->isTalking = menu->TalkToNPC(window, *friendlyNPC, *this);
                    }

                    if (dist > 15.f) friendlyNPC->isTalking = false;
                }
            }



            // DRAWING ITEMS 
            floor->draw(view, projection);

            cube->draw(view, projection);

            player->draw(view, projection);



            for (auto& playerAttack : player->getPlayerProj()) {
                playerAttack->draw(view, projection); //draw the projectiles
            }

            for (auto& object : worldObjectsVec) {
                object->draw(view, projection);
            
            }

#ifdef _DEBUG
            houseFrame->draw(view, projection);
            playerFrame->draw(view, projection); ///debugging
#endif // DEBUG

            i = 0; ///debugging
            for (auto& npc : NPCVec) {
                npc->draw(view, projection);
#ifdef _DEBUG
                NPCWireFrameVec[i]->draw(view, projection); ///debugging
                i++; ///debugging
#endif // _DEBUG
            }
            
            i = 0; ///debugging
            for (auto& sheep : sheepVec) {
                sheep->draw(view, projection);

#ifdef _DEBUG
                sheepWireFrameVec[i]->draw(view, projection); ///debugging
                i++; ///debugging
#endif // _DEBUG
            }

            i = 0; ///debugging
            for (auto& coin : coinsVec) {
                coin->draw(view, projection);
#ifdef _DEBUG
                coinWireFrameVec[i]->draw(view, projection); ///debugging
                i++; ///debugging
#endif // _DEBUG
            }

            menu->HUD(*player, sheepVec, NPCVec);

            friendlyNPC->draw(view, projection);

            //check fps
            double currentTime = glfwGetTime();

            if (currentTime - prevTime >= 1) {
                fps = frames / (currentTime - prevTime);
                frames = 0;
                prevTime = currentTime;

                std::string Title = "fps counter - " + std::to_string(fps);
                glfwSetWindowTitle(window, Title.c_str());
            }


            }
        else if (state == Gamestate::PausedMenu) {

             state = menu->pauseMenu(window); //run the pause menu

        }

        else if (state == Gamestate::Settings) {
            state = menu->settingsMenu(*player, window); //run the settings menu
        }

        else if (state == Gamestate::MainMenu) { 
            state = menu->mainMenu(window, this); //run the main menu

        }
        else if (state == Gamestate::Inventory) {
            

            state = menu->inventoryMenu(*this); //run the inventory menu
        }

        if (state == Gamestate::Dead) {

            if (!DeathScreen->checkFinished()) {
                glm::mat4 view = glm::mat4(1.0f);
                glm::mat4 projection = glm::mat4(1.0f);

                DeathScreen->update();
                DeathScreen->draw(projection, view);

             
            }
            else {
                menu->DeathScreen("unknown");
            }
 
        }

        //have this last after every function which could change gamestate
        if (state == Gamestate::Quit) { SaveLoad::save(this);  break; }



        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        

        //check events and continue. (and display on screen)
        glfwSwapBuffers(window);
        glfwPollEvents();


        if ((float)glfwGetTime() - lastSave >= 30 && state != Gamestate::Dead) {

#ifdef _DEBUG 
            float firstTime = glfwGetTime();
#endif
            SaveLoad::save(this);
            lastSave = (float)glfwGetTime();
#ifdef _DEBUG
            std::cout << "save took: " << lastSave - firstTime << " seconds \n";
#endif // _DEBUG

        }

    }
}