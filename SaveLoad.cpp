#include "SaveLoad.h"
#include <filesystem>


void SaveLoad::save(Manager* m) {

    //get the password
    std::string fileName = m->getPlayer()->getName() + "_SaveGame.json"; 

    std::ifstream readFile(fileName);
    if (!readFile.is_open()) {
        return;
    }

    json LoadData;
    readFile >> LoadData;
    readFile.close();
    auto temp = LoadData["data"];
    std::string password = temp["password"];
    
    json dataJSON;
    dataJSON["password"] = password;

	json playerJSON;
	auto& player = m->getPlayer();
	glm::vec3 playerPos = player->getPlayerPosition();
	playerJSON["position"] = { playerPos.x, playerPos.y, playerPos.z };
	playerJSON["health"] = player->getHealth();
	playerJSON["yaw"] = player->getYaw();
	playerJSON["money"] = player->getMoney();
    playerJSON["name"] = player->getName();

	json inventoryJSON = json::array();

	playerJSON["inventory"] = inventoryJSON;

	json wovlesJSON = json::array();

	for (auto& wolf : m->getNPC()) {
		json NPCjson;
		glm::vec3 pos = wolf->getPosition();
		NPCjson["position"] = { pos.x, pos.y, pos.z };
		NPCjson["health"] = wolf->getHealth();
		wovlesJSON.push_back(NPCjson);
	}

	json sheepJSON = json::array();

	for (auto& sheep : m->getSheep()) {
		json NPCjson;
		glm::vec3 pos = sheep->getPosition();
		NPCjson["position"] = { pos.x,pos.y,pos.z };
		NPCjson["health"] = sheep->getHealth(); //even though its 1 so if i change it
		sheepJSON.push_back(NPCjson);
	}

	json coinJSON = json::array();
	for (auto& coin : m->getCoinVector()) {
		json coinjson;
		glm::vec3 pos = coin->getPosition();
		coinjson["position"] = { pos.x,pos.y,pos.z };
		coinJSON.push_back(coinjson);

	}

	json settingsJSON;
	auto& settings = m->getPlayer()->getSettings();
	settingsJSON["forward"] = settings->getForwards();
	settingsJSON["backwards"] = settings->getBackwards();
	settingsJSON["left"] = settings->getLeft();
	settingsJSON["right"] = settings->getRight();
	settingsJSON["jump"] = settings->getJump();
	settingsJSON["pause"] = settings->getPause();
	settingsJSON["inventory"] = settings->getInventory();

	json SaveData;
	SaveData["player"] = playerJSON;
	SaveData["inventory"] = inventoryJSON;
	SaveData["wolves"] = wovlesJSON;
	SaveData["sheep"] = sheepJSON;
	SaveData["coin"] = coinJSON;
	SaveData["settings"] = settingsJSON;
    SaveData["data"] = dataJSON;

	std::ofstream file(fileName);
	if (file.is_open()) {
		file << SaveData.dump(4);
		file.close();
	}

}

void SaveLoad::load(Manager* m) {

    std::string fileName = m->getPlayer()->getName() + "_SaveGame.json";

    std::ifstream file(fileName);
    if (!file.is_open()) {
        return;
    }

    auto sharedShader = std::make_shared<Shader>("Shaders/Player.vert.txt", 
        "Shaders/Player.frag.txt");

    json SaveData;
    file >> SaveData;
    file.close();


    auto& player = m->getPlayer();
    if (SaveData.contains("player")) {
        auto& playerJSON = SaveData["player"];
        auto posArray = playerJSON["position"];
        player->setPosition(glm::vec3(posArray[0], posArray[1], posArray[2]));
        player->setHealth(playerJSON["health"]);
        player->setYaw(playerJSON["yaw"]);
        player->setMoney(playerJSON["money"]);
        player->setName(playerJSON["name"]);
    }

    //wolves
    if (SaveData.contains("wolves")) {
		m->getNPC().clear(); //clear the current wolves
        for (auto& wolfJSON : SaveData["wolves"]) {
            glm::vec3 pos(wolfJSON["position"][0], wolfJSON["position"][1], wolfJSON["position"][2]);
            int health = wolfJSON["health"];
            m->getNPC().emplace_back(std::make_shared<Wolf>("wolf", sharedShader, pos, 5, loadModel("Wolf.glb")));
        }
    }

    //sheep
    if (SaveData.contains("sheep")) {
        m->getSheep().clear(); //clear the sheep
        for (auto& sheepJSON : SaveData["sheep"]) {
            glm::vec3 pos(sheepJSON["position"][0], sheepJSON["position"][1], sheepJSON["position"][2]);
            int health = sheepJSON["health"];
            m->getSheep().emplace_back(std::make_shared<Sheep>("Sheep", sharedShader, pos, loadModel("Sheep.glb")));
        }
    }

    //coins
    if (SaveData.contains("coin")) {
        m->getCoinVector().clear(); 
        for (auto& coinJSON : SaveData["coin"]) {
            glm::vec3 pos(coinJSON["position"][0], coinJSON["position"][1], coinJSON["position"][2]);
            m->getCoinVector().emplace_back(std::make_shared<Coin>(sharedShader, pos));
        }
    }

    //settings
    if (SaveData.contains("settings")) {
        auto& settingsJSON = SaveData["settings"];
        auto& settings = player->getSettings();
        settings->setForwards(settingsJSON["forward"]);
        settings->setBackwards(settingsJSON["backwards"]);
        settings->setLeft(settingsJSON["left"]);
        settings->setRight(settingsJSON["right"]);
        settings->setJump(settingsJSON["jump"]);
        settings->setPause(settingsJSON["pause"]);
        settings->setInventory(settingsJSON["inventory"]);
    }

}

//std::string SaveLoad::loadName() { //this is un-needed, just use the player name now
//    std::ifstream file("saveGame.json");
//
//    if (!file.is_open()) {
//        return "no Name";
//    }
//
//    json saveData;
//    file >> saveData;
//    file.close();
//    auto& playerJSON = saveData["player"];
//    return playerJSON["name"];
//}



char SaveLoad::encrypt(char x, int value) {
    return x - value; 
}

char SaveLoad::decrypt(char x, int value) {
    return x + value;
}

int SaveLoad::calculateHash(std::string name) {
    int sum = 0;
    for (char c : name) {
        sum += c;
    }

    return (sum % 5) + 1;
}


bool SaveLoad::createAccount(std::string name, std::string password) {

    if (name.size() == 0 || password.size() == 0) {
        throw std::runtime_error("3");
    }

    if (name.find('<') != name.npos || name.find('>') != name.npos || name.find(':') != name.npos ||
        name.find('/') != name.npos || name.find('|') != name.npos || name.find('?') != name.npos ||
        name.find('*') != name.npos || name.find('.') != name.npos) 
    {
        throw std::runtime_error("2"); //illegal characters
    }


    int hashValue = SaveLoad::calculateHash(name); //calculate the hash
    
    std::string fileName = name + "_SaveGame.json"; //make the name
   
    if (std::filesystem::exists(fileName)) { //already exists
        throw std::runtime_error("1");
    }

    std::ofstream file(fileName); //create the file

    std::string hashPassword = "";

    for (char c : password) { //for each loop
        hashPassword += SaveLoad::encrypt(c, hashValue);
    }

    json data;
    data["password"] = hashPassword;

    json saveData;
    saveData["data"] = data;
    file << saveData; //need to not overwrite this - store the hashed password
    
    return true;
}

//may have to use exceptions and i have to change a lot of the code
//need to put a place for password

bool SaveLoad::logIn(std::string name, std::string password) {

    //if the file exists
    std::string fileName = name + "_SaveGame.json";

    if (!std::filesystem::exists(fileName)) {
        throw std::runtime_error("2");
    }

    //calculate the encyrpted password
    int encryptedVal = SaveLoad::calculateHash(name);
    std::string enteredPasswordEncrypted = "";
    for (char c : password) {
        enteredPasswordEncrypted += SaveLoad::encrypt(c, encryptedVal);
    }

    //getting the encrypted password
    std::ifstream file(fileName);
    json LoadData;
    file >> LoadData;
    file.close();
    auto temp = LoadData["data"];
    std::string EncryptedPassword = temp["password"];

    //check if they match
    if (enteredPasswordEncrypted == EncryptedPassword) {
        return true;
    }
    throw std::runtime_error("1");
}

