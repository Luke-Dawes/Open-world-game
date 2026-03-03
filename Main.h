#pragma once

enum class Gamestate { Playing, PausedMenu, MainMenu, Settings, Inventory, Quit, Dead };

extern Gamestate state; //tell the rest of the files that there is a vairable called state which is of type gamestate