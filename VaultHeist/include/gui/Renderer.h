#pragma once
#include "../containers/DynArray.h"

using namespace std;

class Renderer {
public:
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
	 Renderer* nlevel;
public:
	Renderer();
	~Renderer();
	Renderer* parser(Renderer* h, int act);
	int levelCounter(int act);
	void display(Renderer* h);
};