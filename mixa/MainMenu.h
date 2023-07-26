#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


using namespace sf;
using namespace std;

#define max_main_menu 3
class MainMenu
{

public:
	MainMenu() = default;
	MainMenu(float width, float height);

	bool isPlayButtonSelected() const {
		// Placeholder logic
		return MainMenuSelected == 0;
	}

	bool isExitButtonSelected() const {
		// Placeholder logic
		return MainMenuSelected == 3;
	}

	void draw(RenderWindow& win);
	void MoveUp();
	void MoveDown();

	int MainMenuPressed() {
		return MainMenuSelected;
	}
private:
	int MainMenuSelected;
	Font font;
	Text mainMenu[max_main_menu];
};