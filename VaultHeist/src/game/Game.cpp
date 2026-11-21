#include "../../include/game/Game.h"
#include "../../include/game/Campaign.h"
#include "../../include/heap/Heap.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <string>

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

								if (c->actID == 4)
								{
									window.close();
									LevelGenerator4(c);
								}
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



// ---------------------------------------------------------------------------
//  game-specific heap helpers (everything generic comes from MaxHeap itself)
//  mode codes used in the literals below:
//    0 build, 1 repair, 2 extract, 3 race, 4 boss
//  a4Evaluate returns: 0 = invalid, 1 = parent/child swap, 2 = extraction
// ---------------------------------------------------------------------------

// index of the larger child of p inside the ACTIVE heap, or -1 if none
static int a4LargerChild(const MaxHeap& h, int p)
{
	int hs = h.getSize();
	int l = 2 * p + 1, r = 2 * p + 2;
	if (l >= hs) return -1;
	if (r >= hs) return l;
	return (h.getElement(r) > h.getElement(l)) ? r : l;
}

// next correct parent/child swap (deepest violating parent = bottom-up order)
static bool a4NextFixSwap(const MaxHeap& h, int& p, int& c)
{
	for (int i = h.getSize() / 2 - 1; i >= 0; i--)
	{
		int ch = a4LargerChild(h, i);
		if (ch != -1 && h.getElement(ch) > h.getElement(i))
		{
			p = i;
			c = ch;
			return true;
		}
	}
	return false;
}

// swap-only optimal step count, simulated on a copy of the starting heap
static int a4OptimalSteps(const MaxHeap& start, int n, int mode)
{
	MaxHeap sim(start);              // copy constructor (never use '=')
	int steps = 0, p, c;

	while (a4NextFixSwap(sim, p, c))
	{
		sim.swapElements(p, c);
		steps++;
	}

	if (mode >= 2)
	{
		for (int hs = n; hs > 1; hs--)
		{
			sim.swapElements(0, hs - 1);
			steps++;
			sim.setSize(hs - 1);
			while (a4NextFixSwap(sim, p, c))
			{
				sim.swapElements(p, c);
				steps++;
			}
		}
	}
	return steps;
}

static void a4FillDistinct(int* a, int n)
{
	for (int i = 0; i < n; i++)
	{
		int v;
		bool dup;
		do
		{
			v = rand() % 90 + 10;
			dup = false;
			for (int j = 0; j < i; j++)
				if (a[j] == v) dup = true;
		} while (dup);
		a[i] = v;
	}
}

// turn a[] into a valid Max Heap using MaxHeap::buildMaxHeap
static void a4MakeHeap(int* a, int n)
{
	MaxHeap t(a, n);
	t.buildMaxHeap();
	for (int i = 0; i < n; i++)
		a[i] = t.getElement(i);
}

// seeded level data so every attempt of a level is reproducible
static void a4Generate(int seed, int mode, int* a, int n)
{
	srand((unsigned int)seed);

	if (mode == 3)                              // race: duplicate-heavy, already a heap
	{
		for (int i = 0; i < n; i++)
			a[i] = 20 + (rand() % 5) * 10;
		a4MakeHeap(a, n);
		return;
	}

	a4FillDistinct(a, n);
	if (mode == 0 || mode == 4)                 // build / boss: unsorted, never a heap
	{
		MaxHeap t(a, n);
		if (t.isMaxHeap())
		{
			int x = a[0]; a[0] = a[n - 1]; a[n - 1] = x;
		}
	}
	else if (mode == 1)                         // repair: valid heap, then damaged
	{
		a4MakeHeap(a, n);
		a[0] = 1 + rand() % 8;                  // too small -> sift-down
		a[n - 1] = 100 + rand() % 20;           // too big   -> sift-up
	}
	else                                        // extract: valid heap
		a4MakeHeap(a, n);
}

// judge a player swap of cells i and j
static int a4Evaluate(const MaxHeap& h, int mode, int i, int j, string& why)
{
	int hs = h.getSize();
	int lo = (i < j) ? i : j;
	int hi = (i < j) ? j : i;
	bool extractPair = (mode >= 2 && hs >= 2 && lo == 0 && hi == hs - 1);

	if (extractPair && h.isMaxHeap())
		return 2;

	if (hi < 1 || (hi - 1) / 2 != lo)
	{
		why = extractPair ? "Fix the heap before extracting." : "Pick a parent and its child.";
		return 0;
	}
	if (h.getElement(hi) <= h.getElement(lo))
	{
		why = "Parent is already >= child.";
		return 0;
	}
	int sib = (hi % 2 == 1) ? hi + 1 : hi - 1;
	if (sib < hs && h.getElement(sib) > h.getElement(hi))
	{
		why = "Swap with the LARGER child.";
		return 0;
	}
	return 1;
}

// ---------------------------------------------------------------------------
//  small SFML helpers
// ---------------------------------------------------------------------------
static FloatRect a4Panel(float x, float y, float w, float h)
{
	// art is 1376 x 768, window is 1365 x 765
	float sx = 1365.0f / 1376.0f, sy = 765.0f / 768.0f;
	return FloatRect(Vector2f(x * sx, y * sy), Vector2f(w * sx, h * sy));
}

static void a4Text(RenderWindow& w, const Font& f, const string& s, unsigned int size,
	Vector2f pos, Color col, bool center = false)
{
	Text t(f, s, size);
	t.setFillColor(col);
	if (center)
	{
		FloatRect b = t.getLocalBounds();
		t.setOrigin(Vector2f(b.position.x + b.size.x / 2.0f, b.position.y + b.size.y / 2.0f));
	}
	t.setPosition(pos);
	w.draw(t);
}

static string a4Wrap(const Font& f, const string& s, unsigned int size, float maxW)
{
	string out, line;
	Text probe(f, "", size);
	size_t pos = 0;
	while (pos < s.size())
	{
		size_t sp = s.find(' ', pos);
		if (sp == string::npos) sp = s.size();
		string word = s.substr(pos, sp - pos);
		pos = sp + 1;

		string trial = line.empty() ? word : line + " " + word;
		probe.setString(trial);
		if (probe.getLocalBounds().size.x > maxW && !line.empty())
		{
			out += line + "\n";
			line = word;
		}
		else
			line = trial;
	}
	out += line;
	return out;
}

static int a4Lines(const string& s)
{
	int n = 1;
	for (size_t i = 0; i < s.size(); i++)
		if (s[i] == '\n') n++;
	return n;
}

static void a4Line(RenderWindow& w, Vector2f p1, Vector2f p2, float th, Color col)
{
	float dx = p2.x - p1.x, dy = p2.y - p1.y;
	float len = sqrtf(dx * dx + dy * dy);
	RectangleShape r(Vector2f(len, th));
	r.setOrigin(Vector2f(0.0f, th / 2.0f));
	r.setPosition(p1);
	r.setRotation(radians(atan2f(dy, dx)));
	r.setFillColor(col);
	w.draw(r);
}

static void a4Box(RenderWindow& w, FloatRect r, Color fill, Color outline, float th)
{
	RectangleShape s(r.size);
	s.setPosition(r.position);
	s.setFillColor(fill);
	s.setOutlineColor(outline);
	s.setOutlineThickness(th);
	w.draw(s);
}

static void a4Star(RenderWindow& w, Vector2f c, float R, Color fill, Color outline)
{
	ConvexShape s(10);
	for (int i = 0; i < 10; i++)
	{
		float ang = -1.5707963f + i * 0.6283185f;
		float r = (i % 2 == 0) ? R : R * 0.45f;
		s.setPoint(i, Vector2f(c.x + r * cosf(ang), c.y + r * sinf(ang)));
	}
	s.setFillColor(fill);
	s.setOutlineColor(outline);
	s.setOutlineThickness(2.0f);
	w.draw(s);
}

static void a4Button(RenderWindow& w, const Font& f, FloatRect r, const string& label,
	bool enabled, bool hover)
{
	const Color gold(255, 205, 70), text(235, 228, 210);
	Color fill = enabled ? (hover ? Color(70, 45, 110, 235) : Color(28, 20, 50, 225)) : Color(25, 25, 25, 200);
	Color edge = enabled ? (hover ? Color(255, 230, 140) : gold) : Color(90, 90, 90);
	a4Box(w, r, fill, edge, 2.0f);
	a4Text(w, f, label, 14, Vector2f(r.position.x + r.size.x / 2.0f, r.position.y + r.size.y / 2.0f - 2.0f),
		enabled ? text : Color(110, 110, 110), true);
}

// ===========================================================================
//  LEVEL GENERATOR FOR ACT 4
// ===========================================================================
void Game::LevelGenerator4(Renderer* c)
{
	// ------------- local constants (no globals, no enums) -------------
	const float A4_W = 1365.0f, A4_H = 765.0f;
	const float A4_SX = 1365.0f / 1376.0f;      // background art is 1376 x 768
	const float A4_SY = 765.0f / 768.0f;
	const int   A4_MAXN = 20;                   // max array size supported
	const float A4_SWAP_TIME = 0.40f;           // swap animation (s)
	const float A4_AUTO_DELAY = 0.20f;          // pause between auto moves (s)
	const float A4_REPLAY_DELAY = 0.30f;        // pause between replay moves (s)
	const float A4_FLASH_TIME = 0.60f;          // invalid-move flash (s)
	const double A4_ALARM_MULT = 1.5;           // capacity = ceil(mult * optimal) + add
	const double A4_ALARM_MULT_RACE = 1.2;      // tighter alarm for the "race" level
	const int   A4_ALARM_ADD = 2;
	const int   A4_BASE_SCORE = 1000;
	const int   A4_PENALTY_POINTS = 20;         // score points lost per invalid move
	const int   A4_THREE_STAR_BONUS = 100;
	const int   A4_LOG_LINES = 8;
	const int   A4_SIZES[5] = { 8, 10, 9, 12, 15 };   // array size per mode

	// level modes (plain ints; must match the literals used in the helpers above)
	const int A4_BUILD = 0, A4_REPAIR = 1, A4_EXTRACT = 2, A4_RACE = 3, A4_BOSS = 4;
	// move results of a4Evaluate
	const int A4_INVALID = 0, A4_SWAP = 1, A4_EXTRACT_MOVE = 2;

	// colours (never the only signal: labels, outlines and marks are used too)
	const Color A4_GOLD(255, 205, 70);
	const Color A4_RED(235, 70, 70);
	const Color A4_GREEN(110, 225, 130);
	const Color A4_CYAN(120, 220, 235);
	const Color A4_TEXT(235, 228, 210);

	// ----------------------------- level mode -------------------------------
	int mode = -1;
	if (c->mode == "build")        mode = A4_BUILD;
	else if (c->mode == "repair")  mode = A4_REPAIR;
	else if (c->mode == "extract") mode = A4_EXTRACT;
	else if (c->mode == "race")    mode = A4_RACE;
	else if (c->mode == "boss")    mode = A4_BOSS;
	if (mode < 0)   // fallback when "mode" is empty/unknown
		mode = (c->levelID >= 1 && c->levelID <= 5) ? c->levelID - 1 : A4_BOSS;

	const int n = A4_SIZES[mode];               // number of treasures

	// ----------------------------- model ------------------------------------
	int initA[A4_MAXN] = { 0 };                 // starting values (reset / replay)
	int bakA[A4_MAXN] = { 0 };                  // backup while a replay runs
	int bakHs = 0;
	MaxHeap heap(initA, n);                     // the game model (filled in resetLevel)

	// ----------------------------- window/assets ----------------------------
	RenderWindow window(VideoMode({ 1365, 765 }), "Gameplay");
	window.setFramerateLimit(60);

	Texture background;
	if (!background.loadFromFile("VaultHeist/assets/background/gameplay.jpeg"))
		return;
	Sprite bg(background);
	bg.setScale(Vector2f(A4_W / (float)background.getSize().x, A4_H / (float)background.getSize().y));

	Font font;
	if (!font.openFromFile("VaultHeist/assets/fonts/lato/Lato-Italic.ttf"))
		return;

	// ----------------------------- panels (art coords) ----------------------
	const FloatRect pLevel = a4Panel(22, 22, 286, 173);
	const FloatRect pObjective = a4Panel(22, 205, 286, 170);
	const FloatRect pScore = a4Panel(1120, 22, 237, 100);
	const FloatRect pAlarm = a4Panel(1122, 210, 234, 117);
	const FloatRect alarmBar = a4Panel(1140, 264, 198, 40);
	const FloatRect pLog = a4Panel(22, 520, 358, 230);
	const FloatRect pOps = a4Panel(428, 636, 519, 114);
	const FloatRect pInstr = a4Panel(995, 520, 360, 230);
	const float titleBar = 42.0f * A4_SY;

	// ----------------------------- game state -------------------------------
	int steps = 0, penalties = 0, alarmCap = 0, optimal = 0;
	bool hintUsed = false, autoUsed = false;
	int hintIdx = 0;
	int selected = -1, hintA = -1, hintB = -1;
	string status, hintText;
	Color statusCol = A4_TEXT;

	DynArray<string> logLines;
	DynArray<int> snaps;      // n values + active heap size per snapshot
	DynArray<int> moves;      // (i, j, type) triples = replay log

	int autoMode = 0;         // 0 none, 1 build/heapify, 2 one extraction, 3 full heap sort
	bool autoExtracted = false, paused = false;
	float autoWait = 0.0f;

	bool replaying = false;
	int replayPos = 0;
	float replayWait = 0.0f;

	bool animOn = false;
	int animI = -1, animJ = -1;
	float animT = 0.0f;
	int flashA = -1, flashB = -1;
	float flashT = 0.0f;

	int state = 0;            // 0 playing, 1 completed, 2 alarm full
	bool resultPending = false, showResult = false;
	int stars = 0, score = 0;
	int exitAction = 0;       // 1 = back to campaign, 2 = next level

	const float nodeR = 22.0f;

	// ----------------------------- small lambdas ----------------------------
	auto addLog = [&](const string& s) { logLines.pushBack(s); };
	auto setStatus = [&](const string& s, Color col) { status = s; statusCol = col; };

	// write values into the heap (sorted region included) and set the active size
	auto loadValues = [&](const int* src, int size)
		{
			heap.setSize(n);
			for (int i = 0; i < n; i++)
				heap.setElement(i, src[i]);
			heap.setSize(size);
		};

	// isSortedAscending() looks at the active region only, so widen it briefly
	auto fullySorted = [&]()
		{
			int keep = heap.getSize();
			heap.setSize(n);
			bool ok = heap.isSortedAscending();
			heap.setSize(keep);
			return ok;
		};

	auto calcScore = [&](bool bonus)
		{
			int s = A4_BASE_SCORE - 10 * (steps + penalties) - penalties * A4_PENALTY_POINTS;
			if (bonus && stars == 3) s += A4_THREE_STAR_BONUS;
			return (s < 0) ? 0 : s;
		};

	auto resetLevel = [&]()
		{
			a4Generate(c->seed, mode, initA, n);
			loadValues(initA, n);
			steps = penalties = 0;
			hintUsed = autoUsed = false;
			hintIdx = 0;
			selected = hintA = hintB = -1;
			hintText = "";
			autoMode = 0; autoExtracted = false; paused = false; autoWait = 0.0f;
			replaying = false; replayPos = 0;
			animOn = false; flashT = 0.0f;
			state = 0; resultPending = showResult = false;
			stars = score = 0;

			optimal = a4OptimalSteps(heap, n, mode);
			double mult = (mode == A4_RACE) ? A4_ALARM_MULT_RACE : A4_ALARM_MULT;
			alarmCap = (int)ceil(mult * optimal) + A4_ALARM_ADD;

			logLines.clear(); snaps.clear(); moves.clear();
			addLog("HEAP_BUILD start (" + to_string(n) + " treasures)");
			setStatus("Click a treasure, then a second one to swap them.", A4_TEXT);
		};

	auto finishLevel = [&]()
		{
			autoMode = 0;
			replaying = false;
			if (state == 1)
			{
				stars = 1;
				if (steps <= 1.25 * optimal && penalties <= 1) stars = 2;
				if (steps <= optimal && penalties == 0) stars = 3;
				if (hintUsed && stars > 2) stars = 2;
				if (autoUsed && stars > 1) stars = 1;
				if (stars > c->stars)
				{
					c->updateStar(c, 4, stars);     // rewrites the JSON
					c->stars = stars;
				}
			}
			else
				stars = 0;
			score = calcScore(state == 1);
			showResult = true;
		};

	auto checkOutcome = [&]()
		{
			bool won = (mode <= A4_REPAIR) ? (heap.getSize() == n && heap.isMaxHeap())
				: (heap.getSize() == 0 && fullySorted());
			if (won)
			{
				state = 1;
				resultPending = true;
				addLog(mode <= A4_REPAIR ? "HEAPIFY_DONE heap restored" : "HEAP_SORT_COMPLETE");
			}
			else if (steps + penalties >= alarmCap)
			{
				state = 2;
				resultPending = true;
				addLog("ALARM FULL - guards arrive");
			}
		};

	// valid move: snapshot, swap, animate, log
	auto doMove = [&](int i, int j, int type)
		{
			int hsNow = heap.getSize();
			for (int k = 0; k < n; k++) snaps.pushBack(heap.getElement(k));
			snaps.pushBack(hsNow);
			moves.pushBack(i); moves.pushBack(j); moves.pushBack(type);

			int vi = heap.getElement(i), vj = heap.getElement(j);
			heap.swapElements(i, j);
			steps++;
			animOn = true; animI = i; animJ = j; animT = 0.0f;
			selected = hintA = hintB = -1;
			addLog("HEAP_SWAP " + to_string(i) + "(" + to_string(vi) + ") <-> " + to_string(j) + "(" + to_string(vj) + ")");

			if (type == A4_EXTRACT_MOVE)
			{
				int last = hsNow - 1;
				addLog("EXTRACT_MAX " + to_string(heap.getElement(last)));
				addLog("HEAP_SIZE_CHANGE " + to_string(hsNow) + " -> " + to_string(hsNow - 1));
				addLog("MARK_SORTED idx " + to_string(last));
				heap.setSize(hsNow - 1);
				if (heap.getSize() == 1)
				{
					addLog("MARK_SORTED idx 0");
					heap.setSize(0);
				}
				setStatus("Max extracted. Now sift the new root down.", A4_GREEN);
			}
			else
				setStatus("Good swap.", A4_GREEN);

			checkOutcome();
			if (state != 0) autoMode = 0;
		};

	auto nextMove = [&](int& i, int& j, int& type) -> bool
		{
			int p, ch;
			if (a4NextFixSwap(heap, p, ch)) { i = p; j = ch; type = A4_SWAP; return true; }
			if (mode >= A4_EXTRACT && heap.getSize() >= 2) { i = 0; j = heap.getSize() - 1; type = A4_EXTRACT_MOVE; return true; }
			return false;
		};

	auto doUndo = [&]()
		{
			int stride = n + 1, cnt = snaps.size();
			if (cnt < stride) { setStatus("Nothing to undo.", A4_TEXT); return; }
			int base = cnt - stride;
			int savedSize = snaps.get(base + n);
			heap.setSize(n);
			for (int k = 0; k < n; k++)
				heap.setElement(k, snaps.get(base + k));
			heap.setSize(savedSize);
			for (int k = 0; k < stride; k++) snaps.popBack();
			for (int k = 0; k < 3; k++) moves.popBack();
			selected = hintA = hintB = -1;
			animOn = false;
			addLog("UNDO (alarm not refunded)");
			setStatus("Undone. The alarm does not go back down.", A4_TEXT);
		};

	auto doHint = [&]()
		{
			hintUsed = true;
			int hc = c->hints.size();
			if (hc > 0) { hintText = c->hints.get(hintIdx % hc); hintIdx++; }
			else hintText = "Look for a parent that is smaller than a child.";

			if (mode != A4_BOSS)
			{
				int i, j, t;
				if (nextMove(i, j, t)) { hintA = i; hintB = j; }
				setStatus("Hint used (max 2 stars). Gold outline = next swap.", A4_GOLD);
			}
			else
				setStatus("Hint used (max 2 stars). No moves are shown on the boss level.", A4_GOLD);
			addLog("HINT used");
		};

	auto startAuto = [&](int m)
		{
			if (m == 1 && heap.isMaxHeap()) { setStatus("The active heap is already valid.", A4_TEXT); return; }
			if (m >= 2 && heap.getSize() < 2) { setStatus("Nothing left to extract.", A4_TEXT); return; }
			autoMode = m; autoExtracted = false; autoUsed = true; paused = false; autoWait = 0.0f;
			selected = hintA = hintB = -1;
			if (m == 1) addLog(heap.getSize() == n ? "HEAP_BUILD (auto)" : "HEAPIFY_START (auto)");
			else if (m == 2) addLog("EXTRACT_MAX (auto)");
			else addLog("HEAP_SORT (auto)");
			setStatus("Auto-play running. This caps the level at 1 star.", A4_GOLD);
		};

	auto startReplay = [&]()
		{
			if (moves.size() < 3) { setStatus("Nothing to replay yet.", A4_TEXT); return; }
			for (int k = 0; k < n; k++) bakA[k] = heap.getElement(k);
			bakHs = heap.getSize();
			loadValues(initA, n);
			replaying = true; replayPos = 0; replayWait = 0.3f; paused = false;
			autoMode = 0; animOn = false; selected = hintA = hintB = -1;
			addLog("REPLAY started");
		};

	auto stopReplay = [&]()
		{
			loadValues(bakA, bakHs);
			replaying = false; animOn = false;
			addLog("REPLAY finished");
			setStatus("Replay finished.", A4_TEXT);
		};

	// the player clicked treasure idx (tree node or array cell)
	auto onPick = [&](int idx)
		{
			if (idx >= heap.getSize()) { setStatus("That treasure is already in the sorted region.", A4_TEXT); return; }
			if (selected == -1) { selected = idx; setStatus("Selected index " + to_string(idx) + ". Click a second treasure.", A4_TEXT); return; }
			if (selected == idx) { selected = -1; setStatus("Selection cleared.", A4_TEXT); return; }

			int i = selected, j = idx;
			selected = -1;
			string why;
			int type = a4Evaluate(heap, mode, i, j, why);
			if (type == A4_INVALID)
			{
				penalties++;
				flashA = i; flashB = j; flashT = A4_FLASH_TIME;
				addLog("INVALID " + why);
				setStatus("INVALID: " + why + " (+1 alarm)", A4_RED);
				if (steps + penalties >= alarmCap)
				{
					state = 2; resultPending = true;
					autoMode = 0;
					addLog("ALARM FULL - guards arrive");
				}
			}
			else
				doMove(i, j, type);
		};

	// ----------------------------- layout -----------------------------------
	const float treeL = 335.0f, treeW = 760.0f, treeT = 75.0f, treeB = 440.0f;
	const float arrCenterX = 690.0f, arrY = 566.0f, arrMaxW = 570.0f;

	auto cellW = [&]() { return std::min(44.0f, arrMaxW / (float)n); };

	auto treePos = [&](int idx) -> Vector2f
		{
			int lvl = 0;
			while (((1 << (lvl + 1)) - 1) <= idx) lvl++;
			int first = (1 << lvl) - 1;
			int slots = 1 << lvl;
			int depth = 1;
			while (((1 << depth) - 1) < n) depth++;
			float x = treeL + ((float)(idx - first) + 0.5f) * treeW / (float)slots;
			float gap = (depth > 1) ? std::min(105.0f, (treeB - treeT) / (float)(depth - 1)) : 0.0f;
			return Vector2f(x, treeT + lvl * gap);
		};

	auto cellPos = [&](int idx) -> Vector2f
		{
			float cw = cellW();
			float left = arrCenterX - cw * n / 2.0f;
			return Vector2f(left + (idx + 0.5f) * cw, arrY);
		};

	auto slotPos = [&](int kind, int idx) -> Vector2f { return kind == 0 ? treePos(idx) : cellPos(idx); };

	// position of an element while the swap animation plays
	auto shownPos = [&](int kind, int idx) -> Vector2f
		{
			Vector2f p = slotPos(kind, idx);
			if (animOn && (idx == animI || idx == animJ))
			{
				float e = animT * animT * (3.0f - 2.0f * animT);
				Vector2f o = slotPos(kind, idx == animI ? animJ : animI);
				p = Vector2f(o.x + (p.x - o.x) * e, o.y + (p.y - o.y) * e);
			}
			return p;
		};

	auto hitTest = [&](Vector2f m) -> int
		{
			float cw = cellW();
			int hsNow = heap.getSize();
			for (int i = 0; i < n; i++)
			{
				if (i < hsNow)
				{
					Vector2f p = treePos(i);
					float dx = m.x - p.x, dy = m.y - p.y;
					if (dx * dx + dy * dy <= nodeR * nodeR) return i;
				}
				Vector2f q = cellPos(i);
				if (fabsf(m.x - q.x) <= cw * 0.5f && fabsf(m.y - q.y) <= 19.0f) return i;
			}
			return -1;
		};

	// ----------------------------- buttons ----------------------------------
	const string btnLabel[8] = { "Build Max Heap", "Heapify", "Extract Max", "Heap Sort",
								 "Undo", "Replay", "Reset Level", "Hint" };
	FloatRect btnRect[8];
	{
		float bw = 118.0f * A4_SX, bh = 32.0f * A4_SY, gx = 6.0f * A4_SX;
		float x0 = pOps.position.x + 12.0f * A4_SX;
		float y0 = pOps.position.y + 32.0f * A4_SY;
		for (int i = 0; i < 8; i++)
			btnRect[i] = FloatRect(Vector2f(x0 + (i % 4) * (bw + gx), y0 + (i / 4) * (bh + 6.0f * A4_SY)),
				Vector2f(bw, bh));
	}

	// result-screen buttons (rebuilt on demand)
	const FloatRect resPanel(Vector2f(402.0f, 170.0f), Vector2f(560.0f, 420.0f));
	FloatRect resRect[3];
	string resLabel[3];
	int resId[3];
	int resCount = 0;
	auto buildResultButtons = [&]()
		{
			resCount = 0;
			resLabel[resCount] = "Retry"; resId[resCount++] = 0;
			if (state == 1 && c->nlevel != nullptr && c->nlevel->actID == 4)
			{
				resLabel[resCount] = "Next Level"; resId[resCount++] = 1;
			}
			resLabel[resCount] = "Back to Campaign"; resId[resCount++] = 2;

			float bw = 160.0f, bh = 42.0f, gap = 14.0f;
			float total = resCount * bw + (resCount - 1) * gap;
			float x = resPanel.position.x + (resPanel.size.x - total) / 2.0f;
			float y = resPanel.position.y + resPanel.size.y - 62.0f;
			for (int i = 0; i < resCount; i++)
				resRect[i] = FloatRect(Vector2f(x + i * (bw + gap), y), Vector2f(bw, bh));
		};

	// ----------------------------- start ------------------------------------
	resetLevel();
	Clock frameClock;

	while (window.isOpen())
	{
		float dt = frameClock.restart().asSeconds();
		if (dt > 0.1f) dt = 0.1f;

		Vector2f mouseW = window.mapPixelToCoords(Mouse::getPosition(window));
		bool busy = animOn || autoMode != 0 || replaying || state != 0;

		// button availability
		bool en[8];
		en[0] = !busy && heap.getSize() == n && !heap.isMaxHeap();
		en[1] = !busy && heap.getSize() > 1 && !heap.isMaxHeap();
		en[2] = !busy && mode >= A4_EXTRACT && heap.getSize() >= 2;
		en[3] = !busy && mode >= A4_EXTRACT && heap.getSize() >= 2;
		en[4] = !busy && snaps.size() >= n + 1;
		en[5] = (!animOn && autoMode == 0 && state == 0 && moves.size() >= 3) || replaying;
		en[6] = state == 0;
		en[7] = !busy;

		// ------------------------------ events ------------------------------
		while (const auto event = window.pollEvent())
		{
			if (event->is<Event::Closed>())
				window.close();

			if (const auto* key = event->getIf<Event::KeyPressed>())
			{
				if (key->code == Keyboard::Key::Escape)
				{
					exitAction = 1;
					window.close();
				}
				else if (showResult) { /* overlay: buttons only */ }
				else if (key->code == Keyboard::Key::Space)
				{
					if (autoMode != 0 || replaying) paused = !paused;
				}
				else if (key->code == Keyboard::Key::Z && en[4]) doUndo();
				else if (key->code == Keyboard::Key::H && en[7]) doHint();
			}

			if (const auto* mouse = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouse->button != Mouse::Button::Left) continue;
				Vector2f m = window.mapPixelToCoords(mouse->position);

				if (showResult)
				{
					buildResultButtons();
					for (int i = 0; i < resCount; i++)
					{
						if (!resRect[i].contains(m)) continue;
						if (resId[i] == 0) resetLevel();
						else if (resId[i] == 1) { exitAction = 2; window.close(); }
						else { exitAction = 1; window.close(); }
						break;
					}
					continue;
				}

				bool onButton = false;
				for (int i = 0; i < 8; i++)
				{
					if (!btnRect[i].contains(m)) continue;
					onButton = true;
					if (!en[i]) break;
					switch (i)
					{
					case 0: case 1: startAuto(1); break;
					case 2: startAuto(2); break;
					case 3: startAuto(3); break;
					case 4: doUndo(); break;
					case 5: if (replaying) stopReplay(); else startReplay(); break;
					case 6: resetLevel(); break;
					case 7: doHint(); break;
					}
					break;
				}

				if (!onButton && !busy)
				{
					int idx = hitTest(m);
					if (idx != -1) onPick(idx);
				}
			}
		}
		if (!window.isOpen()) break;

		// ------------------------------ update ------------------------------
		if (flashT > 0.0f) flashT -= dt;

		if (animOn)
		{
			animT += dt / A4_SWAP_TIME;
			if (animT >= 1.0f) { animT = 1.0f; animOn = false; }
		}

		if (!animOn && resultPending)
		{
			resultPending = false;
			finishLevel();
		}

		// auto-play
		if (!animOn && !replaying && autoMode != 0 && !paused && state == 0)
		{
			autoWait -= dt;
			if (autoWait <= 0.0f)
			{
				int i = 0, j = 0, t = A4_SWAP;
				bool go = false;
				int p, ch;
				if (a4NextFixSwap(heap, p, ch)) { i = p; j = ch; t = A4_SWAP; go = true; }
				else if (autoMode == 1) go = false;
				else if (autoMode == 2 && autoExtracted) go = false;
				else if (mode >= A4_EXTRACT && heap.getSize() >= 2)
				{
					i = 0; j = heap.getSize() - 1; t = A4_EXTRACT_MOVE; go = true; autoExtracted = true;
				}

				if (go)
				{
					doMove(i, j, t);
					autoWait = A4_AUTO_DELAY;
				}
				else
				{
					autoMode = 0;
					addLog("AUTO complete");
					setStatus("Auto-play finished.", A4_TEXT);
				}
			}
		}

		// replay
		if (replaying && !animOn && !paused)
		{
			replayWait -= dt;
			if (replayWait <= 0.0f)
			{
				if (replayPos * 3 < moves.size())
				{
					int i = moves.get(replayPos * 3), j = moves.get(replayPos * 3 + 1), t = moves.get(replayPos * 3 + 2);
					heap.swapElements(i, j);
					if (t == A4_EXTRACT_MOVE)
					{
						heap.setSize(heap.getSize() - 1);
						if (heap.getSize() == 1) heap.setSize(0);
					}
					animOn = true; animI = i; animJ = j; animT = 0.0f;
					replayPos++;
					replayWait = A4_REPLAY_DELAY;
				}
				else
					stopReplay();
			}
		}

		// ------------------------------ draw --------------------------------
		const int hs = heap.getSize();          // active heap size for this frame

		window.clear(Color::Black);
		window.draw(bg);

		// readability backings
		a4Box(window, FloatRect(Vector2f(328.0f, 28.0f), Vector2f(774.0f, 452.0f)), Color(8, 6, 20, 120), Color(0, 0, 0, 0), 0.0f);
		a4Box(window, FloatRect(Vector2f(392.0f, 498.0f), Vector2f(596.0f, 124.0f)), Color(8, 6, 20, 160), Color(0, 0, 0, 0), 0.0f);

		// ---- Level information panel
		{
			float x = pLevel.position.x + 14.0f, y = pLevel.position.y + titleBar + 4.0f;
			a4Text(window, font, "Act 4 - The Heap Fortress", 14, Vector2f(x, y), A4_GOLD);
			y += 20.0f;
			string lv = a4Wrap(font, "Level 4." + to_string(c->levelID) + ": " + c->levelName, 14, pLevel.size.x - 28.0f);
			a4Text(window, font, lv, 14, Vector2f(x, y), A4_TEXT);
			y += 19.0f * a4Lines(lv) + 4.0f;
			a4Text(window, font, "Steps: " + to_string(steps) + "   Penalties: " + to_string(penalties), 14, Vector2f(x, y), A4_TEXT);
			y += 19.0f;
			a4Text(window, font, "Alarm used: " + to_string(steps + penalties) + " / " + to_string(alarmCap), 14, Vector2f(x, y), A4_TEXT);
			y += 19.0f;
			a4Text(window, font, "Active heap size: " + to_string(hs) + " / " + to_string(n), 14, Vector2f(x, y), A4_TEXT);
		}

		// ---- Objective panel
		{
			string goal;
			if (mode == A4_BUILD) goal = "Goal: every parent must be >= its children.";
			else if (mode == A4_REPAIR) goal = "Goal: restore the heap property (sift up and sift down).";
			else if (mode == A4_EXTRACT) goal = "Goal: extract every maximum into the sorted region.";
			else if (mode == A4_RACE) goal = "Goal: finish Heap Sort before the alarm fills.";
			else goal = "Goal: sort the array ascending to open the final vault.";
			float x = pObjective.position.x + 14.0f, y = pObjective.position.y + titleBar + 4.0f;
			string txt = a4Wrap(font, c->mission, 13, pObjective.size.x - 28.0f) + "\n\n" +
				a4Wrap(font, goal, 13, pObjective.size.x - 28.0f);
			a4Text(window, font, txt, 13, Vector2f(x, y), A4_TEXT);
		}

		// ---- Score panel
		{
			Vector2f ctr(pScore.position.x + pScore.size.x / 2.0f, pScore.position.y + titleBar + 20.0f);
			a4Text(window, font, "Score: " + to_string(calcScore(false)), 26, ctr, A4_GOLD, true);
			a4Text(window, font, "Best stars: " + to_string(c->stars) + " / 3", 13,
				Vector2f(ctr.x, ctr.y + 28.0f), A4_TEXT, true);
		}

		// ---- Alarm panel
		{
			float ratio = (alarmCap > 0) ? (float)(steps + penalties) / (float)alarmCap : 0.0f;
			if (ratio > 1.0f) ratio = 1.0f;
			Color fill = ratio < 0.5f ? Color(90, 200, 110) : (ratio < 0.8f ? Color(235, 195, 60) : A4_RED);
			a4Box(window, alarmBar, Color(15, 12, 25, 220), A4_GOLD, 2.0f);
			a4Box(window, FloatRect(alarmBar.position, Vector2f(alarmBar.size.x * ratio, alarmBar.size.y)), fill, Color(0, 0, 0, 0), 0.0f);
			string t = to_string(steps + penalties) + " / " + to_string(alarmCap);
			if (ratio >= 0.8f) t += "  DANGER";
			a4Text(window, font, t, 14, Vector2f(alarmBar.position.x + alarmBar.size.x / 2.0f, alarmBar.position.y + alarmBar.size.y + 12.0f), A4_TEXT, true);
		}

		// ---- Event log panel
		{
			float x = pLog.position.x + 14.0f, y = pLog.position.y + titleBar + 4.0f;
			int cnt = logLines.size();
			int start = (cnt > A4_LOG_LINES) ? cnt - A4_LOG_LINES : 0;
			for (int k = start; k < cnt; k++)
			{
				string s = logLines.get(k);
				Color col = A4_TEXT;
				if (s.rfind("INVALID", 0) == 0 || s.rfind("ALARM", 0) == 0) col = A4_RED;
				else if (s.rfind("HEAP_SORT_COMPLETE", 0) == 0 || s.rfind("HEAPIFY_DONE", 0) == 0) col = A4_GREEN;
				else if (s.rfind("EXTRACT_MAX", 0) == 0 || s.rfind("MARK_SORTED", 0) == 0) col = A4_GOLD;
				a4Text(window, font, s, 13, Vector2f(x, y), col);
				y += 20.0f;
			}
		}

		// ---- Instruction panel
		{
			float x = pInstr.position.x + 14.0f, y = pInstr.position.y + titleBar + 2.0f;
			float w = pInstr.size.x - 28.0f;
			string how;
			if (mode == A4_BUILD) how = "Swap a parent with its LARGER child when that child is bigger. Work from the last non-leaf upwards.";
			else if (mode == A4_REPAIR) how = "Sift big values up and small values down until no child is bigger than its parent.";
			else how = "Swap the root with the last ACTIVE cell to extract it, then sift the new root down.";
			how = a4Wrap(font, how, 13, w);
			a4Text(window, font, how, 13, Vector2f(x, y), Color(190, 185, 170));
			y += 17.0f * a4Lines(how) + 6.0f;

			string st = status;
			Color sc = statusCol;
			if (replaying) { st = "REPLAY " + to_string(replayPos) + " / " + to_string(moves.size() / 3) + (paused ? " (paused)" : ""); sc = A4_CYAN; }
			else if (autoMode != 0) { st = string("AUTO-PLAY running") + (paused ? " (paused)" : "") + " - max 1 star"; sc = A4_GOLD; }
			st = a4Wrap(font, st, 14, w);
			a4Text(window, font, st, 14, Vector2f(x, y), sc);
			y += 18.0f * a4Lines(st) + 6.0f;

			if (!hintText.empty())
			{
				string h = a4Wrap(font, "Hint: " + hintText, 13, w);
				a4Text(window, font, h, 13, Vector2f(x, y), A4_GOLD);
			}
			a4Text(window, font, "Click = select | Z undo | H hint | Space pause | Esc menu", 11,
				Vector2f(x, pInstr.position.y + pInstr.size.y - 22.0f), Color(150, 145, 135));
		}

		// ---- Heap tree view
		{
			// edges (red when a child is larger than its parent)
			for (int i = 1; i < hs; i++)
			{
				int p = (i - 1) / 2;
				bool bad = heap.getElement(i) > heap.getElement(p);
				a4Line(window, treePos(p), treePos(i), bad ? 4.0f : 3.0f, bad ? A4_RED : Color(170, 140, 80));
			}
			if (hs > 0)
				a4Text(window, font, "MAX (root)", 13, Vector2f(treePos(0).x, treePos(0).y - nodeR - 12.0f), A4_GOLD, true);

			for (int i = 0; i < n; i++)
			{
				bool ghost = (i >= hs);
				if (ghost && !(animOn && i == animJ)) continue;      // extracted: only seen in the array
				Vector2f p = shownPos(0, i);
				bool bad = (!ghost && i > 0 && heap.getElement(i) > heap.getElement((i - 1) / 2));
				Color fill = ghost ? Color(85, 65, 25) : Color(25, 50, 85);
				Color edge = ghost ? A4_GOLD : A4_CYAN;
				float th = 2.0f;
				if (bad) { edge = A4_RED; th = 4.0f; }
				if (i == hintA || i == hintB) { edge = A4_GOLD; th = 5.0f; }
				if (i == selected) { fill = Color(40, 110, 200); edge = Color::White; th = 5.0f; }
				if (flashT > 0.0f && (i == flashA || i == flashB)) { fill = Color(150, 30, 30); edge = A4_RED; th = 5.0f; }

				CircleShape ci(nodeR);
				ci.setOrigin(Vector2f(nodeR, nodeR));
				ci.setPosition(p);
				ci.setFillColor(fill);
				ci.setOutlineColor(edge);
				ci.setOutlineThickness(th);
				window.draw(ci);
				a4Text(window, font, to_string(heap.getElement(i)), 16, p, A4_TEXT, true);
				if (bad)
					a4Text(window, font, "!", 18, Vector2f(p.x + nodeR * 0.95f, p.y - nodeR * 0.95f), A4_RED, true);
			}
		}

		// ---- Array view
		{
			float cw = cellW();
			float left = arrCenterX - cw * n / 2.0f;

			if (hs > 0)
			{
				a4Line(window, Vector2f(left, arrY - 30.0f), Vector2f(left + cw * hs, arrY - 30.0f), 2.0f, A4_CYAN);
				a4Text(window, font, "ACTIVE HEAP (size " + to_string(hs) + ")", 13,
					Vector2f(left + cw * hs / 2.0f, arrY - 42.0f), A4_CYAN, true);
			}
			if (hs < n)
			{
				float sl = left + cw * hs;
				a4Line(window, Vector2f(sl, arrY - 30.0f), Vector2f(left + cw * n, arrY - 30.0f), 2.0f, A4_GOLD);
				a4Text(window, font, "SORTED", 13, Vector2f(sl + cw * (n - hs) / 2.0f, arrY - 42.0f), A4_GOLD, true);
			}

			for (int i = 0; i < n; i++)
			{
				Vector2f p = shownPos(1, i);
				bool sorted = (i >= hs);
				bool bad = (!sorted && i > 0 && heap.getElement(i) > heap.getElement((i - 1) / 2));
				Color fill = sorted ? Color(85, 65, 25) : Color(25, 50, 85);
				Color edge = sorted ? A4_GOLD : A4_CYAN;
				float th = 2.0f;
				if (bad) { edge = A4_RED; th = 3.0f; }
				if (i == hintA || i == hintB) { edge = A4_GOLD; th = 4.0f; }
				if (i == selected) { fill = Color(40, 110, 200); edge = Color::White; th = 4.0f; }
				if (flashT > 0.0f && (i == flashA || i == flashB)) { fill = Color(150, 30, 30); edge = A4_RED; th = 4.0f; }

				FloatRect r(Vector2f(p.x - cw / 2.0f + 1.5f, p.y - 19.0f), Vector2f(cw - 3.0f, 38.0f));
				a4Box(window, r, fill, edge, th);
				if (sorted)     // double border = sorted region (not only colour)
					a4Box(window, FloatRect(Vector2f(r.position.x + 4.0f, r.position.y + 4.0f), Vector2f(r.size.x - 8.0f, r.size.y - 8.0f)),
						Color(0, 0, 0, 0), A4_GOLD, 1.0f);
				a4Text(window, font, to_string(heap.getElement(i)), 16, p, A4_TEXT, true);
				if (bad)
					a4Text(window, font, "!", 15, Vector2f(p.x, p.y - 26.0f), A4_RED, true);
				a4Text(window, font, to_string(i), 11, Vector2f(slotPos(1, i).x, arrY + 29.0f), Color(170, 165, 150), true);
			}
		}

		// ---- Operation buttons
		for (int i = 0; i < 8; i++)
		{
			string lbl = btnLabel[i];
			if (i == 5 && replaying) lbl = "Stop Replay";
			a4Button(window, font, btnRect[i], lbl, en[i], btnRect[i].contains(mouseW));
		}

		// ---- Tooltip
		if (!showResult)
		{
			int hov = hitTest(mouseW);
			if (hov != -1)
			{
				string tip = "index " + to_string(hov) + " | value " + to_string(heap.getElement(hov)) + (hov < hs ? " | active" : " | sorted");
				if (hov > 0) tip += " | parent idx " + to_string((hov - 1) / 2);
				Vector2f tp(mouseW.x + 14.0f, mouseW.y + 14.0f);
				a4Box(window, FloatRect(Vector2f(tp.x - 4.0f, tp.y - 2.0f), Vector2f(tip.size() * 7.0f + 10.0f, 22.0f)),
					Color(10, 8, 20, 235), A4_GOLD, 1.0f);
				a4Text(window, font, tip, 13, tp, A4_TEXT);
			}
		}

		// ---- Result overlay
		if (showResult)
		{
			buildResultButtons();
			a4Box(window, FloatRect(Vector2f(0.0f, 0.0f), Vector2f(A4_W, A4_H)), Color(0, 0, 0, 170), Color(0, 0, 0, 0), 0.0f);
			a4Box(window, resPanel, Color(20, 14, 40, 245), A4_GOLD, 3.0f);

			float cx = resPanel.position.x + resPanel.size.x / 2.0f;
			float y0 = resPanel.position.y;
			a4Text(window, font, state == 1 ? "VAULT CRACKED!" : "ALARM TRIGGERED!", 30,
				Vector2f(cx, y0 + 38.0f), state == 1 ? A4_GREEN : A4_RED, true);

			for (int s = 0; s < 3; s++)
				a4Star(window, Vector2f(cx + (s - 1) * 70.0f, y0 + 92.0f), 24.0f,
					s < stars ? A4_GOLD : Color(40, 36, 55), s < stars ? Color(255, 240, 170) : Color(120, 115, 130));
			a4Text(window, font, "Stars: " + to_string(stars) + " / 3", 16, Vector2f(cx, y0 + 130.0f), A4_TEXT, true);

			float ly = y0 + 156.0f;
			string detail[4] = {
				"Steps: " + to_string(steps) + "   (optimal: " + to_string(optimal) + ")",
				"Penalties: " + to_string(penalties),
				"Alarm used: " + to_string(steps + penalties) + " / " + to_string(alarmCap),
				"Score: " + to_string(score) };
			for (int i = 0; i < 4; i++)
				a4Text(window, font, detail[i], 17, Vector2f(cx, ly + i * 24.0f), A4_TEXT, true);

			if (state == 1)
			{
				string arr = (mode <= A4_REPAIR) ? "Final heap: " : "Sorted array: ";
				for (int i = 0; i < n; i++) arr += to_string(heap.getElement(i)) + " ";
				arr = a4Wrap(font, arr, 14, resPanel.size.x - 40.0f);
				a4Text(window, font, arr, 14, Vector2f(cx, y0 + 285.0f), A4_GOLD, true);

				if (mode == A4_BOSS)
				{
					FloatRect badge(Vector2f(cx - 150.0f, y0 + 318.0f), Vector2f(300.0f, 36.0f));
					a4Box(window, badge, Color(60, 45, 10), A4_GOLD, 3.0f);
					a4Text(window, font, "FINAL VAULT UNLOCKED", 18, Vector2f(cx, badge.position.y + 17.0f), A4_GOLD, true);
				}
			}

			for (int i = 0; i < resCount; i++)
				a4Button(window, font, resRect[i], resLabel[i], true, resRect[i].contains(mouseW));
		}

		window.display();
	}

	// ----------------------------- leaving -----------------------------------
	if (exitAction == 1)
	{
		Campaign camp;
		camp.Act4();
	}
	else if (exitAction == 2 && c->nlevel != nullptr)
		LevelDescription(c->nlevel);
}