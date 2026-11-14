#include "../../include/game/MainMenu.h"

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

using namespace sf;

void MainMenu::startingScreen()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Starting Screen");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/starting_screen.jpeg"))
		return;
	Sprite b(background);

	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
		{
			window.close();
			MainScreen();
		}

		window.clear(Color::Black);

		window.draw(b);

		window.display();
	}
}


void MainMenu::MainScreen()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Main Screen");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/main_screen.jpeg"))
		return;
	Sprite b(background);

	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
				window.close();
		}

		window.clear(Color::Black);

		window.draw(b);

		window.display();
	}
}