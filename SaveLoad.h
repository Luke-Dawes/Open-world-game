#pragma once
#include "json.hpp"
#include "GameManager.h"

using json = nlohmann::json;

namespace SaveLoad {

	void save(Manager* m);
	void load(Manager* m);

	//std::string loadName();

	int calculateHash(std::string name); //calculate the hash from the name and return it
	char encrypt(char x, int hashingValue); //if i decrease the char value by hasingValue - decrease makes it fit into ascii instead of overflowing
	char decrypt(char x, int hashingValue); //i just need to increase it here 

	bool logIn(std::string name, std::string username); //check against a file - or maybe check if the save file exists
	bool createAccount(std::string name, std::string username); //create the file empty? so then i just write over it 

	//need to change the load file to {player_name}_Save.json
}
