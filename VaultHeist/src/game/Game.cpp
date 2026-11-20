#include "../../include/game/Game.h"
#include "../../include/game/Campaign.h"
#include <SFML/Graphics.hpp>

using namespace sf;

void Game::LevelDescription(Renderer* c)
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Level Description");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/level-description.jpeg"))
		return;

	Sprite b(background);

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/level-description-buttons.png"))
		return;

	Sprite button[2] = { Sprite(btn), Sprite(btn) };
	for (int i = 0; i < 2; i++)
		button[i].setTextureRect(IntRect({ 90, 280 + (i * 460) }, { 718, 268 }));

	//rectangle boxes 
	
	RectangleShape box[3];
	for (int i = 0; i < 3; i++)
	{
		box[i].setSize(Vector2f(710, 70));
		box[i].setPosition(Vector2f(130, 310 + i*130));
		box[i].setFillColor(Color::Transparent);
	}

	//Description texts
	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;

	Text s[3] = { Text(font),Text(font), Text(font) };
	for (int i = 0; i < 3; i++)
	{
		s[i].setString((i == 0) ? c->levelName : (i == 1) ? c->description : c->mission);
		FloatRect bounds = s[i].getLocalBounds();

		s[i].setOrigin(Vector2f(bounds.position.x + bounds.size.x / 2.0f, 0));

		s[i].setPosition(
			Vector2f(box[i].getPosition().x + box[i].getSize().x / 2.0f,
				box[i].getPosition().y + 10)
		);
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
					for (int i = 0; i < 2; i++)
						if (button[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)))
						{
							if (i == 1)
							{
								window.close();
								Campaign camp;
								(c->actID == 1) ? camp.Act1() : (c->actID == 2) ? camp.Act2() : (c->actID == 3) ? camp.Act3() : camp.Act4();
							}
							else
							{
								//ayesha's level logic
							}
						}
				}
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if (button[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)))
			{
				button[i].setScale(Vector2f(0.55, 0.55));
				button[i].setPosition(Vector2f(100 + (i * 400), 640));
			}
			else
			{
				button[i].setScale(Vector2f(0.5, 0.5));
				button[i].setPosition(Vector2f(110 + (i * 400), 640));
			}
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button[0]);
		window.draw(button[1]);
		for (int i = 0; i < 3; i++)
		{
			window.draw(s[i]);
			window.draw(box[i]);
		}

		window.display();
	}
}