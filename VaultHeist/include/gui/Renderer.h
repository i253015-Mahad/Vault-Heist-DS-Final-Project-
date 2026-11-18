#include <fstream>
#include <iostream>
#include <string>
#include "../containers/DynArray.h"

using namespace std;

class Renderer {
private:
	float levelID;
	string levelName;
	int actID;
	string structure;
	string mission;
	string description;
	int seed;
	 DynArray<string> hints;
	 bool completed;
	 int stars;
	 //for some acts
	 int targetKey;
	 DynArray<int> initalKeys;
public:
	Renderer();
	void parser(int act);
	void levelCounter(int act);
};

//implementation

Renderer::Renderer()
{
	levelID = actID = seed = targetKey = stars =  0;
	levelName = mission = structure = description = "";
	completed = false;
}

void Renderer::parser(int act)
{
	string line = "", item = "";
	int c = 0;
	bool hinty = false, inikeys = false;
	ifstream p;
	switch (act)  
	{
	case 1: p.open("VaultHeist/data/levels/Act1.json"); break;
	case 2: p.open("VaultHeist/data/levels/Act2.json"); break;
	case 3: p.open("VaultHeist/data/levels/Act3.json"); break;
	case 4: p.open("VaultHeist/data/levels/Act4.json"); break;
	}
	if (!p)
		return;

	while (getline(p, line))
	{
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
					levelID = stof(item);
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
					levelName = item;
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
					actID = stoi(item);
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
					structure = item;
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
					mission = item;
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
					description = item;
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
					seed = stoi(item);
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
					hints.pushBack(item);
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
					targetKey = stoi(item);
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
					initalKeys.pushBack(stoi(item));
					if (line[c] != ',')
						inikeys = false;
				}
			}
			c++;
		}
	}
	p.close();
	cout << levelID << "\n" << levelName << "\n" << actID << "\n" << structure << "\n" << mission << "\n" << description;
	cout << "\n" << seed << "\n"<<targetKey<<endl;
	hints.display();
	initalKeys.display();
}

void Renderer::levelCounter(int act)
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
		return;

	while (getline(p, line))
	{
		if (line == "{")
			open = true;
		else if (open && line == "}")
		{
			count++;
			open = false;
		}
	}

	cout << "Total levels: " << count;
}