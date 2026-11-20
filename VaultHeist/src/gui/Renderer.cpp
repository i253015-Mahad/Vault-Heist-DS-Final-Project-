#include <fstream>
#include <iostream>
#include <string>
#include "../../include/gui/Renderer.h"

//implementation

Renderer::Renderer()
{
	levelID = actID = seed = targetKey = stars = 0;
	levelName = mission = structure = description = "";
	completed = false;
	nlevel = nullptr;
}

Renderer::~Renderer()
{
	if (nlevel == nullptr)
		return;
	delete nlevel;
	nlevel = nullptr;
}

Renderer* Renderer::parser(Renderer* h, int act)
{
	Renderer* curr = h;
	string line = "", item = "";
	int c = 0;
	bool hinty = false, inikeys = false, nl = false;
	ifstream p;
	switch (act)
	{
	case 1: p.open("VaultHeist/data/levels/Act1.json"); break;
	case 2: p.open("VaultHeist/data/levels/Act2.json"); break;
	case 3: p.open("VaultHeist/data/levels/Act3.json"); break;
	case 4: p.open("VaultHeist/data/levels/Act4.json"); break;
	}
	if (!p)
		return h;

	while (getline(p, line))
	{
		if (line == "  },")
			nl = true;
		if (line == "  {" && nl)
		{
			curr->nlevel = new Renderer;
			curr = curr->nlevel;
		}
		c = 0;
		item = "";
		while (line[c] != '\0')
		{
			if (line[c] != ' ' && line[c] != '\"')
			{
				item += line[c];
				if (item == "levelId")
				{
					while (line[c] < '1' || line[c] > '9')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != ' ' && line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->levelID = stof(item);
				}
				else if (item == "levelName")
				{
					while (line[c] < 'A' || line[c] > 'Z')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->levelName = item;
				}

				else if (item == "actId")
				{
					while (line[c] < '1' || line[c] > '9')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != ' ' && line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->actID = stoi(item);
				}

				else if (item == "dataStructure")
				{
					while (line[c] < 'A' || line[c] > 'Z')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->structure = item;
				}

				else if (item == "mission")
				{
					while (line[c] < 'A' || line[c] > 'Z')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->mission = item;
				}

				else if (item == "description")
				{
					while (line[c] < 'A' || line[c] > 'Z')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->description = item;
				}

				else if (item == "seed")
				{
					while (line[c] < '1' || line[c] > '9')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != ' ' && line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->seed = stoi(item);
				}

				else if (item == "hints")
				{
					hinty = true;
					break;
				}
				else if (hinty)
				{
					while (line[c] < 'A' || line[c] > 'Z')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->hints.pushBack(item);
					if (line[c] != ',')
						hinty = false;
				}

				else if (item == "targetKey")
				{
					while (line[c] < '1' || line[c] > '9')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != ' ' && line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->targetKey = stoi(item);
				}

				else if (item == "initialKeys")
				{
					inikeys = true;
					break;
				}

				else if (inikeys)
				{
					while (line[c] < '0' || line[c] > '9')
						c++;
					item = "";
					while (line[c] != '\0')
					{
						if (line[c] != ' ' && line[c] != '\"' && line[c] != ',')
							item += line[c];
						c++;
					}
					c--;
					curr->initalKeys.pushBack(stoi(item));
					if (line[c] != ',')
						inikeys = false;
				}
			}
			c++;
		}
	}
	p.close();
	return h;
}

int Renderer::levelCounter(int act)
{
	string line = "";
	bool open = false;
	int count = 0;
	ifstream p;
	switch (act)
	{
	case 1: p.open("VaultHeist/data/levels/Act1.json"); break;
	case 2: p.open("VaultHeist/data/levels/Act2.json"); break;
	case 3: p.open("VaultHeist/data/levels/Act3.json"); break;
	case 4: p.open("VaultHeist/data/levels/Act4.json"); break;
	}
	if (!p)
		return -1;

	while (getline(p, line))
	{
		if (line == "  {")
			open = true;
		else if (open && (line == "  }" || line == "  },"))
		{
			count++;
			open = false;
		}
	}

	return count;
}

void Renderer::display(Renderer* h)
{
	Renderer* curr = h;
	while (curr != nullptr)
	{
		cout << "ActID: " << curr->actID << "\nLevelID: " << curr->levelID << "\nLevelName: " << curr->levelName;
		cout << "\nDescription: " << curr->description << "\nMission: " << curr->mission << "\nHints:  ";
		curr->hints.display();
		cout << "\nSeed: " << curr->seed << "\nStars: " << curr->stars << "\nStrcture: " << curr->structure;

		cout << "\nTarget Key: " << curr->targetKey << "\nInitial Keys: ";
		curr->initalKeys.display();
		cout << "\n\n";
		curr = curr->nlevel;
	}
}