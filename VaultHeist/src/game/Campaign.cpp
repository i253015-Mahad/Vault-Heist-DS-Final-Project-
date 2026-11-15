#include "../../include/game/Campaign.h"
#include "../../include/game/MainMenu.h"
#include <SFML/Graphics.hpp>

using namespace sf;

void Campaign::CampaignScreen()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Campaign Screen");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/campaign_screen.jpeg"))
		return;

	Sprite b(background);

	Texture lock;
	if (!lock.loadFromFile("VaultHeist/assets/background/campaign_lock.png"))
		return;

	Sprite l[3] = { Sprite(lock), Sprite(lock), Sprite(lock) };
	for (int i = 0; i < 3; i++)
		l[i].setScale(Vector2f(0.2, 0.2));
	l[0].setPosition(Vector2f(980, 280));
	l[1].setPosition(Vector2f(180, 280));
	l[2].setPosition(Vector2f(620, 440));

	Texture name;
	if (!name.loadFromFile("VaultHeist/assets/background/act_names.png"))
		return;

	Sprite acts[4] = { Sprite(name), Sprite(name)  ,Sprite(name) , Sprite(name) };
	for (int i = 0; i < 4; i++)
	{
		acts[i].setTextureRect(IntRect({ 50, 90 + (i * 200) }, { 1100, 80 }));
		acts[i].setScale(Vector2f(0.4, 0.4));
	}
	acts[0].setPosition(Vector2f(380, 50));
	acts[1].setPosition(Vector2f(880, 220));
	acts[2].setPosition(Vector2f(30, 250));
	acts[3].setPosition(Vector2f(520, 650));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/campaign_back.png"))
		return;

	Sprite button(btn);
	button.setScale(Vector2f(0.15, 0.15));
	button.setPosition(Vector2f(30, 20));

	bool open[4] = { true, false, false, false };
	bool once[4] = { false, false, false, false };

	RectangleShape bound[4];
	for (int i = 0; i < 4; i++)
	{
		bound[i].setSize(Vector2f(400, 200));
		bound[i].setFillColor(Color::Transparent);
	}
	bound[0].setPosition(Vector2f(400, 50));
	bound[1].setPosition(Vector2f(880, 220));
	bound[2].setPosition(Vector2f(70, 270));
	bound[3].setPosition(Vector2f(520, 450));

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
					for (int i = 0; i < 4; i++)
						if (bound[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)) && open[i])
							switch (i)
							{
							case 0: window.close(); Act1(); break; //act 1
							case 1: window.close(); Act2(); break; //act 2
							case 2: window.close(); Act3(); break; //act 3
							case 3: window.close(); Act4(); break; //act 4
							}
					if (button.getGlobalBounds().contains(
						window.mapPixelToCoords(mousepos)))
					{
						window.close();
					    MainMenu m;
						m.MainScreen();
					}
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			Vector2f x = acts[i].getPosition();
			if (bound[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)) && open[i])
			{
				acts[i].setScale(Vector2f(0.5, 0.5));
				if (!once[i])
				{
					acts[i].setPosition(Vector2f(x.x - 50, x.y));
					once[i] = true;
				}
			}
			else
			{
				acts[i].setScale(Vector2f(0.4, 0.4));
				if (once[i])
				{
					acts[i].setPosition(Vector2f(x.x + 50, x.y));
					once[i] = false;
				}
			}

		}
		if (button.getGlobalBounds().contains(
			window.mapPixelToCoords(mousepos)))
		{
			button.setScale(Vector2f(0.16, 0.16));
			button.setPosition(Vector2f(20, 20));
		}
		else
		{
			button.setScale(Vector2f(0.15, 0.15));
			button.setPosition(Vector2f(30, 20));
		}

		window.clear(Color::Black);

		window.draw(b);
		for (int i = 0; i < 3; i++)
			if (!open[i+1])
				window.draw(l[i]);
		for (int i = 0; i < 4; i++)
			window.draw(acts[i]);
		for (int i = 0; i < 4; i++)
			window.draw(bound[i]);
		window.draw(button);

		window.display();
	}
}


void Campaign::Act1()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Act 1");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act1.jpeg"))
		return;

	Sprite b(background);
	b.setScale(Vector2f(1.34, 1.34));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);
	button.setScale(Vector2f(0.15, 0.15));
	button.setPosition(Vector2f(560, 680));

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
						CampaignScreen();
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

void Campaign::Act2()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Act 2");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act2.jpeg"))
		return;

	Sprite b(background);
	b.setScale(Vector2f(1.34, 1.34));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);
	button.setScale(Vector2f(0.15, 0.15));
	button.setPosition(Vector2f(560, 660));

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
						CampaignScreen();
					}
				}
			}
		}

		if (button.getGlobalBounds().contains(
			window.mapPixelToCoords(mousepos)))
		{
			button.setScale(Vector2f(0.16, 0.16));
			button.setPosition(Vector2f(550, 660));
		}
		else
		{
			button.setScale(Vector2f(0.15, 0.15));
			button.setPosition(Vector2f(560, 660));
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);

		window.display();
	}
}

void Campaign::Act3()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Act 3");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act3.jpeg"))
		return;

	Sprite b(background);
	b.setScale(Vector2f(1.34, 1.34));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);
	button.setScale(Vector2f(0.15, 0.15));
	button.setPosition(Vector2f(560, 670));

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
						CampaignScreen();
					}
				}
			}
		}

		if (button.getGlobalBounds().contains(
			window.mapPixelToCoords(mousepos)))
		{
			button.setScale(Vector2f(0.16, 0.16));
			button.setPosition(Vector2f(550, 670));
		}
		else
		{
			button.setScale(Vector2f(0.15, 0.15));
			button.setPosition(Vector2f(560, 670));
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);

		window.display();
	}
}

void Campaign::Act4()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Act 4");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act4.jpeg"))
		return;

	Sprite b(background);
	b.setScale(Vector2f(1.34, 1.34));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);
	button.setScale(Vector2f(0.15, 0.15));
	button.setPosition(Vector2f(540, 660));

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
						CampaignScreen();
					}
				}
			}
		}

		if (button.getGlobalBounds().contains(
			window.mapPixelToCoords(mousepos)))
		{
			button.setScale(Vector2f(0.16, 0.16));
			button.setPosition(Vector2f(530, 660));
		}
		else
		{
			button.setScale(Vector2f(0.15, 0.15));
			button.setPosition(Vector2f(540, 660));
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);

		window.display();
	}
}