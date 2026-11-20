#include "../../include/game/Game.h"
#include "../../include/game/Campaign.h"
#include <SFML/Graphics.hpp>

using namespace sf;

void Game::LevelDescription(Renderer* c)
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Level Description");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act1.jpeg"))
		return;

	Sprite b(background);

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);

	//level numbers
	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;

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
					if (button.getGlobalBounds().contains(
						window.mapPixelToCoords(mousepos)))
					{
						window.close();
						Campaign camp;
						(c->actID == 1) ? camp.Act1() : (c->actID == 2) ? camp.Act2() : (c->actID == 3) ? camp.Act3() : camp.Act4();
					}
				}
			}
		}
		if (button.getGlobalBounds().contains(
			window.mapPixelToCoords(mousepos)))
		{
			button.setScale(Vector2f(0.16, 0.16));
			button.setPosition(Vector2f(550, 680));
		}
		else
		{
			button.setScale(Vector2f(0.15, 0.15));
			button.setPosition(Vector2f(560, 680));
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);

		window.display();
	}
}