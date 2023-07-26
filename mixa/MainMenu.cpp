#include "MainMenu.h"

MainMenu::MainMenu(float width, float height) {
	if (!font.loadFromFile("Vogue.ttf")) {
		cout << "no photo";
	}
	//play
	mainMenu[0].setFont(font);
	mainMenu[0].setFillColor(Color::White);
	mainMenu[0].setString("Play");
	mainMenu[0].setCharacterSize(40);
	mainMenu[0].setPosition(400, 200);

	//About
	mainMenu[1].setFont(font);
	mainMenu[1].setFillColor(Color::White);
	mainMenu[1].setString("About");
	mainMenu[1].setCharacterSize(40);
	mainMenu[1].setPosition(400, 300);

	//Exit
	mainMenu[2].setFont(font);
	mainMenu[2].setFillColor(Color::White);
	mainMenu[2].setString("Exit");
	mainMenu[2].setCharacterSize(40);
	mainMenu[2].setPosition(400, 400);

	MainMenuSelected = -1;
}

void MainMenu::draw(RenderWindow& win) {
	for (int i = 0; i < max_main_menu; i++) {
		win.draw(mainMenu[i]);
	}

	Text title("Main Menu", font, 60);
	title.setPosition(300, 100);
	win.draw(title);
}

void MainMenu::MoveUp() {
	if (MainMenuSelected - 1 >= 0) {
		mainMenu[MainMenuSelected].setFillColor(Color::White);

		MainMenuSelected--;

		if (MainMenuSelected == -1) {
			MainMenuSelected = 2;
		}
		mainMenu[MainMenuSelected].setFillColor(Color::Blue);
	}
}

void MainMenu::MoveDown() {
	if (MainMenuSelected + 1 <= max_main_menu) {
		mainMenu[MainMenuSelected].setFillColor(Color::White);

		MainMenuSelected++;

		if (MainMenuSelected == 3) {
			MainMenuSelected = 0;
		}
		mainMenu[MainMenuSelected].setFillColor(Color::Blue);
	}
}