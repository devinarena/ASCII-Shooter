#include<iostream>
#include<thread>
#include<windows.h>
#include<vector>
#include<cmath>
#include<string>

#include "utils.h"
#include "entity.h"
#include "player.h"
#include "boss.h"
#include "boss_hand.h"
#include "shield.h"
#include "spawner.h"

/*
	File	: shooter.cpp
	Author	: Devin Arena
	Date	: 5/6/2021
	Purpose	: Create a shooter game that utilizes only the command line interface to
			  draw graphics. Hopefully these design constraints lead to some unique
			  ideas for gameplay.
*/

// whether we are still running or not
volatile bool running{ true };

// wchar_t* is for actual screen to be drawn to the console
wchar_t* screen = new wchar_t[WIDTH * HEIGHT];
// handle on the screen buffer to write to
HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
// how much has been written
DWORD dwBytesWritten{ 0 };

// list of pointers to all entity (game) objects
std::vector<Entity*> entities = std::vector<Entity*>();

// player pointer, the first element of the vector is the player
Player* player = nullptr;

// boss pointer, second element of the vector will be the boss
Helios* boss = nullptr;

// spawner object, spawns enemies every so often
Spawner* spawner = nullptr;

// score counter, also used to determine when boss comes out
int score{ 0 };
bool hardmode{ false }; // at half the score enter 'hardmode', more enemies and faster shooting

// Forward declarations

void start();
void drawHUD();
void render();
void enterHardmode();
void spawnBoss();
void handleInput();
void cleanup();
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

/**
	Main entry point, simply starts the game.
*/
int main(int argc, char** argv) {
	start();
	return 0;
}

/**
	Start function, seeds the RNG, sets the active screen buffer and
	starts the game loop, lastly cleaning up on exit.
*/
void start() {
	srand(time(NULL));
	SetConsoleActiveScreenBuffer(hConsole);
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	SetConsoleCursorPosition(hConsole, { WIDTH - 1, 0 });

	player = new Player(8, HEIGHT / 2);
	entities.push_back(player);

	spawner = new Spawner();

	while (running) {
		// Render everything to the console
		render();

		// Sleep for desired FPS
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
	}

	cleanup();
}

/**
	Renders characters representing entities and interfaces to the screen.
*/
void render() {
	// handle input
	handleInput();

	// clear the screen
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		screen[i] = L' ';
	}

	// draw the Heads Up Display
	drawHUD();

	// check if the boss has not yet spawned and keep spawning enemies if so
	if (score < REQUIRED_SCORE)
		spawner->update(entities);

	// spawn the boss if we have the required score and he is not alive yet
	if (score >= REQUIRED_SCORE && boss == nullptr) {
		spawnBoss();
	}

	// loop over every entity
	for (int i = 0; i < entities.size(); i++) {
		Entity* e = entities[i];
		// if the entity is dead, remove it
		if (e->isDead()) {
			// except for the player and boss
			if (e->getType() == EntityType::PLAYER || e == boss) {
				continue;
			}
			// add to the score, if the required score is reached enter hardmode
			if (e->getType() == EntityType::ENEMY) {
				if (score < REQUIRED_SCORE / 2 && score + e->getMaxHealth() >= REQUIRED_SCORE / 2)
					enterHardmode();
				score += e->getMaxHealth();
			}
			// remove the entity from the list
			entities.erase(entities.begin() + i--);
			// delete the pointer to the entity
			delete e;
			e = nullptr;
			continue;
		}
		// update the entity
		e->update(entities);

		// bounds checking for the entity (don't render offscreen entities)
		if (e->getX() < 0 || e->getX() >= WIDTH || e->getY() < 0 || e->getY() >= HEIGHT)
			continue;

		// render the entity
		e->render(screen);
	}

	WriteConsoleOutputCharacter(hConsole, screen, WIDTH * HEIGHT, { 0,0 }, &dwBytesWritten);
}

/**
	Renders the HUD to the screen, including health, score, and distance.

	A little hacky but rendering text to the screen using std::cout is
	impossible when we overwrite the entire screen.
*/
void drawHUD() {
	// default array containing " HP: "
	char top[WIDTH] = { ' ', 'H', 'P', ':', '[' };
	// adding a + for each hitpoint followed by -
	int length = player->getMaxHealth();
	for (auto i = 0; i < length; i++) {
		if (i < player->getHealth())
			top[5 + i] = '+';
		else
			top[5 + i] = '-';
	}
	// generate the score text
	top[5 + length] = ']';
	top[6 + length] = ' ';
	top[7 + length] = 'S';
	top[7 + length + 1] = 'C';
	top[7 + length + 2] = 'O';
	top[7 + length + 3] = 'R';
	top[7 + length + 4] = 'E';
	top[7 + length + 5] = ':';
	top[7 + length + 6] = ' ';
	std::string temp = std::to_string(score);
	for (auto i = 0; i < temp.size(); i++)
		top[14 + length + i] = temp[i];
	// now create the bottom indicator
	// the bottom indicator is either a progress bar if the boss is not out yet
	// or a healthbar for the boss
	// find location for player indicator
	char bottom[WIDTH] = { };
	if (score < REQUIRED_SCORE) {
		bottom[0] = '[';
		int pos = 1 + (int)((WIDTH - 2) * ((double)score / REQUIRED_SCORE));
		if (pos > WIDTH - 2)
			pos = WIDTH - 2;
		for (auto i = 1; i < pos; i++) {
			bottom[i] = '-';
		}
		for (auto i = pos + 1; i < WIDTH - 2; i++)
			bottom[i] = '.';
		bottom[pos] = '>';
		bottom[WIDTH - 2] = 'B';
		bottom[WIDTH - 1] = ']';
	}
	// draw the boss healthbar
	else if (boss != nullptr) {
		if (boss->getHealth() < 1) {
			char word[] = "YOU WIN!";
			for (auto i = 0; i < 8; i++)
				bottom[WIDTH / 2 - 4 + i] = word[i];
		}
		else {

			bottom[0] = 'B';
			bottom[1] = 'O';
			bottom[2] = 'S';
			bottom[3] = 'S';
			bottom[4] = ':';
			bottom[5] = '[';
			double perc = 0;
			char hpi = '@';
			// draw shield health
			if (boss->getShield() != nullptr) {
				perc = (double)boss->getShield()->getHealth() / HELIOS::SHIELD_HEALTH;
				hpi = '#';
			}
			// draw normal health
			else {
				double totalHP = boss->getHealth();
				if (boss->getTopHand() != nullptr)
					totalHP += boss->getTopHand()->getHealth();
				if (boss->getBottomHand() != nullptr)
					totalHP += boss->getBottomHand()->getHealth();
				perc = totalHP / (HELIOS::HEALTH + 2 * HELIOS::HAND_HEALTH);
			}
			int hp = (int)round((WIDTH - 1) * perc);
			for (auto i = 6; i < WIDTH - 1; i++) {
				if (i <= hp)
					bottom[i] = hpi;
				else
					bottom[i] = '-';
			}
			bottom[WIDTH - 1] = ']';
		}
	}
	// show if player won
	else {
	}
	// draw to screen
	for (auto i = 0; i < WIDTH; i++) {
		screen[i] = top[i];
	}
	for (auto i = 0; i < WIDTH; i++) {
		screen[WIDTH * (HEIGHT - 1) + i] = bottom[i];
	}
}

/**
	Simply sets the game to hardmode, changing values
	for spawnrate and firing speed.
*/
void enterHardmode() {
	hardmode = true;
	PLAYER::SHOOT_SPEED = 21;
	PLAYER::SHOOT_TIME = 220;
	spawner->setSpawnTime(spawner->getSpawnTime() / 2);
}

/**
	Spawns the boss.
*/
void spawnBoss() {
	for (auto i = 0; i < entities.size(); i++) {
		Entity* e = entities[i];
		if (e->getType() != EntityType::PLAYER) {
			e->setMaxHealth(0);
			e->setHealth(0);
		}
	}
	boss = new Helios();
	HeliosHand* top = new HeliosHand();
	HeliosHand* bottom = new HeliosHand();
	boss->setTopHand(top);
	boss->setBottomHand(bottom);
	boss->newShield(entities);
	top->flipImage();
	top->setBoss(boss);
	bottom->setBoss(boss);
	entities.insert(entities.begin() + 1, boss);
	entities.push_back(top);
	entities.push_back(bottom);
}

/**
	Cleans up used memory by deleting dynamically allocated memory.
*/
void cleanup() {
	delete player;
	delete spawner;

	for (Entity* e : entities) {
		e = nullptr;
		delete e;
	}

	delete screen;
}

/**
	Handles window signals, such as CTRL-C and pressing the X to terminate the program.
	This stops the program from running so memory can be safely cleaned up before exiting.

	@param fdwCtrlType signal to handle
	@return BOOL if the signal was handled
*/
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal.
	case CTRL_C_EVENT:
		running = false;
		return TRUE;
		// CTRL-CLOSE: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		running = false;
		return TRUE;
	default:
		return FALSE;
	}
}

/**
	Enum for keys, just a simple enum for lookups
	to be used in the handleInput function.
*/
enum Keys {
	A = 0x41,
	D = 0x44,
	W = 0x57,
	S = 0x53,
	SPACE = VK_SPACE
};

/**
	Handles input for the application.
	Most keys map to player actions.
*/
void handleInput() {
	if (GetAsyncKeyState(Keys::A) & 0x8000)
		player->setVX(-PLAYER::H_SPEED);
	if (GetAsyncKeyState(Keys::D) & 0x8000)
		player->setVX(PLAYER::H_SPEED);
	if (GetAsyncKeyState(Keys::W) & 0x8000)
		player->setVY(-PLAYER::V_SPEED);
	if (GetAsyncKeyState(Keys::S) & 0x8000)
		player->setVY(PLAYER::V_SPEED);
	if (GetAsyncKeyState(Keys::SPACE) & 0x8000)
		player->setShooting(true);
	else
		player->setShooting(false);
}