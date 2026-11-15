#include "../../include/game/MainMenu.h"
#include "../../include/game/Campaign.h"

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

	Texture options;
	if (!options.loadFromFile("VaultHeist/assets/background/main_options.png"))
		return;

	Sprite op[6] = { Sprite(options), Sprite(options),Sprite(options) ,Sprite(options) ,Sprite(options) ,Sprite(options) };
	for (int i = 0; i < 6; i++)
	{
		op[i].setTextureRect(IntRect({ 180, 180 + (i * 140) }, { 680, 120 }));
		op[i].setScale(Vector2f(0.5, 0.5));
		op[i].setPosition(Vector2f(510, 350 + (i * 70)));
	}

	while (window.isOpen())
	{
		Vector2i mousepos = Mouse::getPosition(window);

		while (const auto event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
				window.close();
	
			if (const auto* mouse = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouse->button == Mouse::Button::Left)
				{
					for (int i = 0; i < 6; i++)
						if (op[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)))
							switch (i)
							{
							case 0: window.close(); Campaign c; c.CampaignScreen(); break; //campaign
							case 1: break; //endless
							case 2: break; //sandbox
							case 3: break; //leaderBoard
							case 4: break; //settings
							case 5: window.close(); break;
							}
				}
			}
		}

		for (int i = 0; i < 6; i++)
		{
			if (op[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)))
			{
				op[i].setScale(Vector2f(0.6, 0.6));
				op[i].setPosition(Vector2f(475, 350 + (i * 70)));
			}
			else
			{
				op[i].setScale(Vector2f(0.5, 0.5));
				op[i].setPosition(Vector2f(510, 350 + (i * 70)));
			}

		}

		window.clear(Color::Black);

		window.draw(b);
		for (int i = 0; i < 6; i++)
			window.draw(op[i]);

		window.display();
	}
}