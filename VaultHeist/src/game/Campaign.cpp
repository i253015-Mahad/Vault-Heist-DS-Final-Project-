#include "../../include/game/Campaign.h"
#include "../../include/game/MainMenu.h"
#include "../../include/gui/Renderer.h"
#include "../../include/game/Game.h"

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
	Renderer* r = new Renderer, * curr;
	for (int i = 1; i < 4; i++)
	{
		r = r->parser(r, i);
		curr = r;
		for (int j = 0; j < r->levelCounter(i)-1; j++)
			curr = curr->nlevel;
		if (curr->stars > 0)
			open[i] = true;
	}
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
	delete r;
	r = nullptr;
	curr = nullptr;
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

	Texture level0, level1, level2, level3, levell;
	if (!level0.loadFromFile("VaultHeist/assets/background/level-0.png") || !level1.loadFromFile("VaultHeist/assets/background/level-1.png") ||
		!level2.loadFromFile("VaultHeist/assets/background/level-2.png") || !level3.loadFromFile("VaultHeist/assets/background/level-3.png") ||
		!levell.loadFromFile("VaultHeist/assets/background/level-locked.png"))
		return;

	Renderer* r = new Renderer;
	int s = r->levelCounter(1);
	if (s > 12)
		s = 12;
	r = r->parser(r, 1);

	bool locked[12] = { false, true, true , true , true , true , true , true , true , true , true , true };

	Sprite ls[12] = {Sprite(level0), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell) };
	for (int i = 0; i < s; i++)
	{
		ls[i].setScale(Vector2f(0.14, 0.14));

		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		
		if (curr->stars > 0)
			locked[i] = false;
		if (curr->stars == 1)
			ls[i].setTexture(level1);
		else if (curr->stars == 2)
			ls[i].setTexture(level2);
		else if (curr->stars == 3)
			ls[i].setTexture(level3);
		else
		{
			if (i != 0)
			{
				Renderer* prev = r;
				while (prev->nlevel != curr)
					prev = prev->nlevel;

				if (prev->stars > 0)
				{
					ls[i].setTexture(level0);
					locked[i] = false;
				}
			}
		}

		curr = nullptr;
	}

	//level numbers
	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;
	Text lnum[12] = { Text(font), Text(font), Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) };
	string o, a = "";
	for (int i = 0; i < s; i++)
	{
		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		o = to_string(curr->levelID);
		lnum[i].setString(o);
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
					if (button.getGlobalBounds().contains(
						window.mapPixelToCoords(mousepos)))
					{
						window.close();
						CampaignScreen();
					}

					for (int i = 0; i < s; i++)
					{
						if (ls[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)) && !locked[i])
						{
							Renderer* curr = r;
							for (int j = 0; j < i; j++)
								curr = curr->nlevel;
							window.close();
							Game g;
							g.LevelDescription(curr);
						}
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
		
		for (int i = 0; i < s; i++)
		{
			if (ls[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)) && !locked[i])
			{
				ls[i].setScale(Vector2f(0.15, 0.15));
				lnum[i].setScale(Vector2f(1.1, 1.1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(382 + i * 140, 280));
					lnum[i].setPosition(Vector2f((461 + i * 140), 320));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-178 + i * 140, 410));
					lnum[i].setPosition(Vector2f((-100 + i * 140), 450));
				}
				else
				{
					ls[i].setPosition(Vector2f(-738 + i * 140, 540));
					lnum[i].setPosition(Vector2f((-669 + i * 140), 580));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-660 + i * 140), 580));
				}
			}
			else
			{
				ls[i].setScale(Vector2f(0.14, 0.14));
				lnum[i].setScale(Vector2f(1, 1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(390 + i * 140, 280));
				    lnum[i].setPosition(Vector2f((465 + i * 140), 320));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-170 + i * 140, 410));
					lnum[i].setPosition(Vector2f((-97 + i * 140), 450));
				}
				else
				{
					ls[i].setPosition(Vector2f(-730 + i * 140, 540));
					lnum[i].setPosition(Vector2f((-665 + i * 140), 580));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-658 + i * 140), 580));
				}
			}
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);
		for (int i = 0; i < s; i++)
		{
			window.draw(ls[i]);
			if (!locked[i])
				window.draw(lnum[i]);
		}

		window.display();
	}
	delete r;
	r = nullptr;
}

void Campaign::Act2()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Act 2");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act2.jpeg"))
		return;

	Sprite b(background);
	b.setScale(Vector2f(1, 1.1));
	b.setPosition(Vector2f(0, -15));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);

	Texture level0, level1, level2, level3, levell;
	if (!level0.loadFromFile("VaultHeist/assets/background/level-0.png") || !level1.loadFromFile("VaultHeist/assets/background/level-1.png") ||
		!level2.loadFromFile("VaultHeist/assets/background/level-2.png") || !level3.loadFromFile("VaultHeist/assets/background/level-3.png") ||
		!levell.loadFromFile("VaultHeist/assets/background/level-locked.png"))
		return;

	Renderer* r = new Renderer;
	int s = r->levelCounter(2);
	if (s > 12)
		s = 12;
	r = r->parser(r, 2);

	bool locked[12] = { false, true, true , true , true , true , true , true , true , true , true , true };

	Sprite ls[12] = { Sprite(level0), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell) };
	for (int i = 0; i < s; i++)
	{

		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		
		if (curr->stars > 0)
			locked[i] = false;
		if (curr->stars == 1)
			ls[i].setTexture(level1);
		else if (curr->stars == 2)
			ls[i].setTexture(level2);
		else if (curr->stars == 3)
			ls[i].setTexture(level3);
		else
		{
			if (i != 0)
			{
				Renderer* prev = r;
				while (prev->nlevel != curr)
					prev = prev->nlevel;

				if (prev->stars > 0)
				{
					ls[i].setTexture(level0);
					locked[i] = false;
				}
			}
		}

		curr = nullptr;
	}

	//level numbers
	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;
	Text lnum[12] = { Text(font), Text(font), Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) };
	string o, a = "";
	for (int i = 0; i < s; i++)
	{
		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		o = to_string(curr->levelID);
		lnum[i].setString(o);
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
					if (button.getGlobalBounds().contains(
						window.mapPixelToCoords(mousepos)))
					{
						window.close();
						CampaignScreen();
					}

					for (int i = 0; i < s; i++)
					{
						if (ls[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)) && !locked[i])
						{
							Renderer* curr = r;
							for (int j = 0; j < i; j++)
								curr = curr->nlevel;
							window.close();
							Game g;
							g.LevelDescription(curr);
						}
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

		for (int i = 0; i < s; i++)
		{
			if (ls[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)) && !locked[i])
			{
				ls[i].setScale(Vector2f(0.15, 0.15));
				lnum[i].setScale(Vector2f(1.1, 1.1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(382 + i * 140, 280));
					lnum[i].setPosition(Vector2f((461 + i * 140), 320));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-178 + i * 140, 410));
					lnum[i].setPosition(Vector2f((-100 + i * 140), 450));
				}
				else
				{
					ls[i].setPosition(Vector2f(-738 + i * 140, 540));
					lnum[i].setPosition(Vector2f((-669 + i * 140), 580));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-660 + i * 140), 580));
				}
			}
			else
			{
				ls[i].setScale(Vector2f(0.14, 0.14));
				lnum[i].setScale(Vector2f(1, 1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(390 + i * 140, 280));
					lnum[i].setPosition(Vector2f((463 + i * 140), 320));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-170 + i * 140, 410));
					lnum[i].setPosition(Vector2f((-97 + i * 140), 450));
				}
				else
				{
					ls[i].setPosition(Vector2f(-730 + i * 140, 540));
					lnum[i].setPosition(Vector2f((-665 + i * 140), 580));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-658 + i * 140), 580));
				}
			}
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);
		for (int i = 0; i < s; i++)
		{
			window.draw(ls[i]);
			if (!locked[i])
				window.draw(lnum[i]);
		}

		window.display();
	}
	delete r;
	r = nullptr;
}

void Campaign::Act3()
{
	RenderWindow window(VideoMode({ 1365, 765 }), "Act 3");

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/act3.jpeg"))
		return;

	Sprite b(background);
	b.setScale(Vector2f(1.05, 1));
	b.setPosition(Vector2f(-40, 0));

	Texture btn;
	if (!btn.loadFromFile("VaultHeist/assets/background/settings_back.png"))
		return;

	Sprite button(btn);
	button.setScale(Vector2f(0.15, 0.15));

	Texture level0, level1, level2, level3, levell;
	if (!level0.loadFromFile("VaultHeist/assets/background/level-0.png") || !level1.loadFromFile("VaultHeist/assets/background/level-1.png") ||
		!level2.loadFromFile("VaultHeist/assets/background/level-2.png") || !level3.loadFromFile("VaultHeist/assets/background/level-3.png") ||
		!levell.loadFromFile("VaultHeist/assets/background/level-locked.png"))
		return;

	Renderer* r = new Renderer;
	int s = r->levelCounter(3);
	if (s > 12)
		s = 12;
	r = r->parser(r, 3);

	bool locked[12] = { false, true, true , true , true , true , true , true , true , true , true , true };

	Sprite ls[12] = { Sprite(level0), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell) };
	for (int i = 0; i < s; i++)
	{
		ls[i].setScale(Vector2f(0.14, 0.14));

		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		
		if (curr->stars > 0)
			locked[i] = false;
		if (curr->stars == 1)
			ls[i].setTexture(level1);
		else if (curr->stars == 2)
			ls[i].setTexture(level2);
		else if (curr->stars == 3)
			ls[i].setTexture(level3);
		else
		{
			if (i != 0)
			{
				Renderer* prev = r;
				while (prev->nlevel != curr)
					prev = prev->nlevel;

				if (prev->stars > 0)
				{
					ls[i].setTexture(level0);
					locked[i] = false;
				}
			}
		}

		curr = nullptr;
	}

	//level numbers
	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;
	Text lnum[12] = { Text(font), Text(font), Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) };
	string o, a = "";
	for (int i = 0; i < s; i++)
	{
		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		o = to_string(curr->levelID);
		lnum[i].setString(o);
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
					if (button.getGlobalBounds().contains(
						window.mapPixelToCoords(mousepos)))
					{
						window.close();
						CampaignScreen();
					}

					for (int i = 0; i < s; i++)
					{
						if (ls[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)) && !locked[i])
						{
							Renderer* curr = r;
							for (int j = 0; j < i; j++)
								curr = curr->nlevel;
							window.close();
							Game g;
							g.LevelDescription(curr);
						}
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

		for (int i = 0; i < s; i++)
		{
			if (ls[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)) && !locked[i])
			{
				ls[i].setScale(Vector2f(0.15, 0.15));
				lnum[i].setScale(Vector2f(1.1, 1.1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(382 + i * 140, 280));
					lnum[i].setPosition(Vector2f((461 + i * 140), 320));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-178 + i * 140, 410));
					lnum[i].setPosition(Vector2f((-100 + i * 140), 450));
				}
				else
				{
					ls[i].setPosition(Vector2f(-738 + i * 140, 540));
					lnum[i].setPosition(Vector2f((-669 + i * 140), 580));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-660 + i * 140), 580));
				}
			}
			else
			{
				ls[i].setScale(Vector2f(0.14, 0.14));
				lnum[i].setScale(Vector2f(1, 1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(390 + i * 140, 280));
					lnum[i].setPosition(Vector2f((463 + i * 140), 320));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-170 + i * 140, 410));
					lnum[i].setPosition(Vector2f((-97 + i * 140), 450));
				}
				else
				{
					ls[i].setPosition(Vector2f(-730 + i * 140, 540));
					lnum[i].setPosition(Vector2f((-665 + i * 140), 580));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-658 + i * 140), 580));
				}
			}
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);
		for (int i = 0; i < s; i++)
		{
			window.draw(ls[i]);
			if (!locked[i])
				window.draw(lnum[i]);
		}

		window.display();
	}
	delete r;
	r = nullptr;
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

	Texture level0, level1, level2, level3, levell;
	if (!level0.loadFromFile("VaultHeist/assets/background/level-0.png") || !level1.loadFromFile("VaultHeist/assets/background/level-1.png") ||
		!level2.loadFromFile("VaultHeist/assets/background/level-2.png") || !level3.loadFromFile("VaultHeist/assets/background/level-3.png") ||
		!levell.loadFromFile("VaultHeist/assets/background/level-locked.png"))
		return;

	Renderer* r = new Renderer;
	int s = r->levelCounter(4);
	if (s > 12)
		s = 12;
	r = r->parser(r, 4);

	bool locked[12] = { false, true, true , true , true , true , true , true , true , true , true , true };

	Sprite ls[12] = { Sprite(level0), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell), Sprite(levell) };
	for (int i = 0; i < s; i++)
	{
		ls[i].setScale(Vector2f(0.14, 0.14));

		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
	
		if (curr->stars > 0)
			locked[i] = false;
		if (curr->stars == 1)
			ls[i].setTexture(level1);
		else if (curr->stars == 2)
			ls[i].setTexture(level2);
		else if (curr->stars == 3)
			ls[i].setTexture(level3);
		else
		{
			if (i != 0)
			{
				Renderer* prev = r;
				while (prev->nlevel != curr)
					prev = prev->nlevel;

				if (prev->stars > 0)
				{
					ls[i].setTexture(level0);
					locked[i] = false;
				}
			}
		}

		curr = nullptr;
	}

	//level numbers
	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;
	Text lnum[12] = { Text(font), Text(font), Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) , Text(font) };
	string o, a = "";
	for (int i = 0; i < s; i++)
	{
		Renderer* curr = r;
		for (int j = 0; j < i; j++)
			curr = curr->nlevel;
		o = to_string(curr->levelID);
		lnum[i].setString(o);
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
					if (button.getGlobalBounds().contains(
						window.mapPixelToCoords(mousepos)))
					{
						window.close();
						CampaignScreen();
					}

					for (int i = 0; i < s; i++)
					{
						if (ls[i].getGlobalBounds().contains(
							window.mapPixelToCoords(mousepos)) && !locked[i])
						{
							Renderer* curr = r;
							for (int j = 0; j < i; j++)
								curr = curr->nlevel;
							window.close();
							Game g;
							g.LevelDescription(curr);
						}
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

		for (int i = 0; i < s; i++)
		{
			if (ls[i].getGlobalBounds().contains(
				window.mapPixelToCoords(mousepos)) && !locked[i])
			{
				ls[i].setScale(Vector2f(0.15, 0.15));
				lnum[i].setScale(Vector2f(1.1, 1.1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(380 + i * 140, 230));
					lnum[i].setPosition(Vector2f((458 + i * 140), 270));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-180 + i * 140, 355));
					lnum[i].setPosition(Vector2f((-100 + i * 140), 400));
				}
				else
				{
					ls[i].setPosition(Vector2f(-740 + i * 140, 480));
					lnum[i].setPosition(Vector2f((-669 + i * 140), 520));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-657 + i * 140), 520));
				}
			}
			else
			{
				ls[i].setScale(Vector2f(0.14, 0.14));
				lnum[i].setScale(Vector2f(1, 1));
				if (i <= 3)
				{
					ls[i].setPosition(Vector2f(385 + i * 140, 230));
					lnum[i].setPosition(Vector2f((457 + i * 140), 270));
				}
				else if (i <= 7)
				{
					ls[i].setPosition(Vector2f(-175 + i * 140, 355));
					lnum[i].setPosition(Vector2f((-103 + i * 140), 400));
				}
				else
				{
					ls[i].setPosition(Vector2f(-735 + i * 140, 480));
					lnum[i].setPosition(Vector2f((-670 + i * 140), 520));
					if (i == 8)
						lnum[i].setPosition(Vector2f((-660 + i * 140), 520));
				}
			}
		}

		window.clear(Color::Black);

		window.draw(b);
		window.draw(button);
		for (int i = 0; i < s; i++)
		{
			window.draw(ls[i]);
			if (!locked[i])
				window.draw(lnum[i]);
		}

		window.display();
	}
	delete r;
	r = nullptr;
}