#include "ImGuiMenus.h"
#include "imgui.h"
#include "SaveLoad.h"
#include "backends\imgui_impl_glfw.h"
#include "backends\imgui_impl_opengl3.h"
#include <string>


int lastKeyPressed = 0;

Gamestate Menu::pauseMenu(GLFWwindow* window) {

    int height, width;
    glfwGetFramebufferSize(window, &height, &width);


    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_size = ImVec2(height, width);

    ImVec2 window_pos = ImVec2(0,0);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);
    ImGui::Begin("Paused", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // makes the title
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Game Paused");

    ImGui::Spacing();
    ImGui::Spacing();

    // makes the menu buttons 
    if (ImGui::Button("Play", ImVec2(300, 50))) { ImGui::End(); return Gamestate::Playing; }

    ImGui::Spacing();

    if (ImGui::Button("Settings", ImVec2(300, 50))) { ImGui::End(); return Gamestate::Settings; }
    

    ImGui::Spacing();

    if (ImGui::Button("Main Menu", ImVec2(300, 50))) { ImGui::End(); return Gamestate::MainMenu; }

    ImGui::Spacing();

    if (ImGui::Button("Quit", ImVec2(300, 50))) { ImGui::End(); return Gamestate::Quit; }

    ImGui::End();

	return Gamestate::PausedMenu;
}

Gamestate Menu::mainMenu(GLFWwindow* window, Manager* m)
{
    static bool showLoadMenu = false;

    int height, width; //add frame buffer call back here
    glfwGetFramebufferSize(window, &height, &width);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 window_size = ImVec2(height, width);

    ImVec2 window_pos = ImVec2((io.DisplaySize.x - window_size.x) / 2, (io.DisplaySize.y - window_size.y) / 2);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);
    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Welcome to the game");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Start game", ImVec2(200, 50))) { ImGui::End(); return Gamestate::Playing; }

    ImGui::Spacing();

    std::string text = "Load game with " + m->getPlayer()->getName();

    if (ImGui::Button(text.c_str(), ImVec2(200, 50))) { SaveLoad::load(m); }

    ImGui::Spacing();

    if (ImGui::Button("Settings", ImVec2(200, 50))) { ImGui::End(); return Gamestate::Settings; }

    ImGui::Spacing();

    if (ImGui::Button("Quit", ImVec2(200, 50))) { ImGui::End(); return Gamestate::Quit; }

    ImGui::End();
    
    return Gamestate::MainMenu;
}

void Menu::HUD(const Player& player, const std::vector<std::shared_ptr<Sheep> >& sheepvec, const std::vector<std::shared_ptr<NPC> >& npcvec)
{
    ImGuiIO& io = ImGui::GetIO();
    int ID = 0;
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowBgAlpha(0.f);

    ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoBackground
        );

    //ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4(0,255,0,0)));


    ImGui::Text("Health: %d", player.getHealth());
    ImGui::Text("Coins: %d", player.getMoney());

    //ImGui::PopStyleColor();

    //player
    ImGui::SetCursorPos(ImVec2(100, 500));

    ImGui::BeginChild("healthBar", ImVec2(100,100), ImGuiWindowFlags_NoTitleBar | 
         ImGuiWindowFlags_NoBackground | 
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoBackground


        );

    float percent = static_cast<float>(player.getHealth()) / player.getMaxHealth();

    ImGui::ProgressBar(percent, ImVec2(500, 50));

    ImGui::EndChild();

    ImGui::End();

}

Gamestate Menu::inventoryMenu(Manager& manager) {
    
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::SetNextWindowSize(ImVec2(500, 600));
    ImGui::SetNextWindowPos(ImVec2(200, 100));
    ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_NoCollapse);

    const auto& items = manager.getInventory()->getInventory();

    if (items.empty()) {
        ImGui::Text("Inventory is empty");
    }
    else {
        ImGui::Text("Items in inventory: ");

        ImGui::Separator();

        for (const auto& itemPtr : items) {
            if (itemPtr) {
                if (ImGui::Button(itemPtr->getName().c_str(), ImVec2(150, 100))) {

                    itemPtr->use(manager);
                }

            }

        }
    }

    if (ImGui::Button("Close", ImVec2(100, 30))) {
        ImGui::CloseCurrentPopup();
        ImGui::End();
        return Gamestate::Playing;
    }


    ImGui::SetCursorPos(ImVec2(250, 50)); //moves where the next child frame begins
    ImGui::BeginChild("current loadout", ImVec2(200, 300)); //creates the child frame and takes in the size
    ImGui::Spacing();
    
    const auto& inventory = manager.getInventory();

    std::string helmet; //creates strings for each thing
    std::string chest;
    std::string boots;
    std::string equipt;


    //checks to see if there is an item there. If there is make the string vairable equal it otherwise make it say none
    helmet = (inventory->getCurrentHelmet() == nullptr) ? "no helmet" : inventory->getCurrentHelmet()->getName();
    chest = (inventory->getCurrentChest() == nullptr) ? "no chest" : inventory->getCurrentChest()->getName();
    boots = (inventory->getCurrentBoots() == nullptr) ? "no boots" : inventory->getCurrentBoots()->getName();
    equipt = (inventory->getCurrentItem() == nullptr) ? "no item" : inventory->getCurrentItem()->getName();

    //prints out the inventory with spaces between them
    //c_str takes a string to a const char* which are C's version of strings
    ImGui::Text("current loadout");
    ImGui::Spacing();
    if (ImGui::Button(helmet.c_str(), ImVec2(200, 50))) {
        if (!(helmet == "no helmet")) { //if helmet doesnt equal the null basic balue

        }
    }
    if (ImGui::Button(chest.c_str(), ImVec2(200, 50))) {
        if (!(chest == "no chest")) {

        }
    }
    if (ImGui::Button(boots.c_str(), ImVec2(200, 50))) {
        if (!(boots == "no boots")) {

        }
    }
    if (ImGui::Button(equipt.c_str(), ImVec2(200, 50))) {
        if (!(equipt == "no item")) {

        }
    }

    ImGui::EndChild();


    ImGui::End(); //ends the main frame

    return Gamestate::Inventory;
}

//use key call backs for key presses

Gamestate Menu::settingsMenu(Player& player, GLFWwindow* window) {

    int height, width;
    glfwSetKeyCallback(window, key_callback);
    glfwGetFramebufferSize(window, &height, &width);
    static bool changeKeyBinds = false;

    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::SetNextWindowSize(ImVec2(height, width));
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Change key binds \n(Press the key and then the button)", ImVec2(300, 100))) {

        changeKeyBinds = !changeKeyBinds;

    }
    ImGui::Spacing();

    if (ImGui::Button("Back to game", ImVec2(300, 100))) {
        ImGui::End(); glfwSetKeyCallback(window, NULL); return Gamestate::Playing;
    }

    if (changeKeyBinds) {
        ImGui::BeginChild("Key Binds", ImVec2(300, 300));

        ImGui::Spacing();
        std::string textForward = "change forward: " + (std::string)glfwGetKeyName(player.getSettings()->getForwards(), 0);
        if (ImGui::Button(textForward.c_str(), ImVec2(300, 50))) {
            player.getSettings()->setForwards(lastKeyPressed);
        }

        ImGui::Spacing();

        std::string textBackwards = "change backwards: " + (std::string)glfwGetKeyName(player.getSettings()->getBackwards(), 0);
        if (ImGui::Button(textBackwards.c_str(), ImVec2(300, 50))) {
            player.getSettings()->setBackwards(lastKeyPressed);
        }

        ImGui::Spacing();

        std::string textLeft = "change Left: " + (std::string)glfwGetKeyName(player.getSettings()->getLeft(), 0);
        if (ImGui::Button(textLeft.c_str(), ImVec2(300, 50))) {
            player.getSettings()->setLeft(lastKeyPressed);
        }

        ImGui::Spacing();

        std::string textRight = "change right: " + (std::string)glfwGetKeyName(player.getSettings()->getRight(), 0);
        if (ImGui::Button(textRight.c_str(), ImVec2(300, 50))) {
            player.getSettings()->setRight(lastKeyPressed);
        }

        ImGui::EndChild();

        
    }
    ImGui::End();
    return Gamestate::Settings;
}

Gamestate Menu::DeathScreen(const std::string& reason) {
    


    //will be called when the screen is black

    
    ImGui::Begin("death", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetCursorPos(ImVec2(200, 200));
    ImGui::Text(("You died beacuse " + reason).c_str());

    ImGui::End();


    return Gamestate::Dead; //needs to change
}

bool Menu::TalkToNPC(GLFWwindow* window, FriendlyNPC& npc, Manager& m)
{

    int height, width;
    glfwGetFramebufferSize(window, &width, &height);

    ImGuiIO& io = ImGui::GetIO();

   
    ImGui::SetNextWindowBgAlpha(1);

    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("diagloue", nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::SetCursorPos(ImVec2(200, 200));
    ImGui::TextWrapped("%s", npc.speak(m.getSheep(), m.getNPC()).c_str()); //print the string which is returned

    ImGui::SetCursorPos(ImVec2(425, 400));
    if (ImGui::Button("Shop", ImVec2(100, 100))) {
        m.shop = true;
    }

    ImGui::SetCursorPos(ImVec2(50, 400));
    if (ImGui::Button("restart\nconvosation", ImVec2(100, 100))) {
        npc.npcDialogueIndex = 0;
    }

    if (npc.npcDialogueIndex == 4) {
		ImGui::SetCursorPos(ImVec2(300, 250));
        ImGui::Text("you have completed the quest");
    }

    ImGui::SetCursorPos(ImVec2(175, 400));
    if (ImGui::Button("exit", ImVec2(100, 100))) {
        ImGui::End();
        return false;
    }

    ImGui::SetCursorPos(ImVec2(300, 400));
    if (ImGui::Button("Next", ImVec2(100, 100))) {
        npc.npcDialogueIndex++;
    }

    if (npc.npcDialogueIndex == 5) {
        ImGui::SetCursorPos(ImVec2(550, 250)); 
        if (ImGui::Button("restart game", ImVec2(100, 100))) {
            m = Manager(m.getCamera(), m.getPlayer()->getName());
            npc.npcDialogueIndex = 0;
            ImGui::End();
            return false;
        }        
    }

    npc.npcDialogueIndex = std::clamp(npc.npcDialogueIndex, 0, (int)npc.getSpeech().size() - 1);
    ImGui::End();
    return true;
}

bool Menu::Shop(GLFWwindow* window, Manager& m) {

    ImGui::Begin("shop", nullptr, ImGuiWindowFlags_NoCollapse);

    int height, width;
    glfwGetFramebufferSize(window, &width, &height); //get width and hight

    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowBgAlpha(1); //black background

    ImGui::SetNextWindowSize(ImVec2(width, height)); //full screen

    ImGui::SetCursorPos(ImVec2(20, 20));
    ImGui::Text("What would you like to buy"); //text

    ImGui::SetCursorPos(ImVec2(50, 50));
    
    const ImColor red = ImColor(255, 0, 0); //set red and green as const vairables
    const ImColor green = ImColor(0, 255, 0);

    auto& player = m.getPlayer();

    ImColor text1Colour = (player->getMoney() >= 10 ? green : red); //set the colour of the text

    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4(text1Colour))); //give the colour to imGui

    ImGui::SetCursorPos(ImVec2(100, 50));
    if (ImGui::Button("Buy health potion. £10")) {
        if (text1Colour == green) { //player has enough money
            player->getInventory()->addToInventory(std::make_shared<HealthPotion>(std::make_shared<Shader>("Shaders/Player.vert.txt",
                "Shaders/Player.frag.txt"), glm::vec3(0, 0, 0), 20));

            player->setMoney(player->getMoney() - 10);
        }

    }

    ImGui::PopStyleColor(); //get rid of the coloured text

    ImColor text2Colour = (player->getMoney() >= 20 ? green : red); //set the colour of the text

    ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4(text2Colour))); //give the colour to imGui

    ImGui::SetCursorPos(ImVec2(100, 100));
    if (ImGui::Button("Buy damage potion. £20")) {
        if (text1Colour == green) { //player has enough money
            player->getInventory()->addToInventory(std::make_shared<DamagePotion>(std::make_shared<Shader>("Shaders/Player.vert.txt",
                "Shaders/Player.frag.txt"), glm::vec3(0, 0, 0), 20));

            player->setMoney(player->getMoney() - 20);
        }

    }

    ImGui::PopStyleColor(); //get rid of the coloured text

    ImGui::SetCursorPos(ImVec2(100, 150));

    if (ImGui::Button("exit")) {
        ImGui::End();
        return false;
    }
    ImGui::End();
    return true;
}