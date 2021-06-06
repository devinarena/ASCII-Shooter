#include "utils.h"

/*
	File	: utils.cpp
	Author	: Devin Arena
	Date	: 5/10/2021
	Purpose	: Contains any necessary constants or functions for the game.
*/

// initialize non integral members
const double PLAYER::SLIDE = 0.8;
// values that will change
int PLAYER::SHOOT_SPEED{ 20 };
int PLAYER::SHOOT_TIME{ 400 };

int HELIOS::CHASE_SPEED = 2;
int HELIOS::SHOOT_TIME_N{ 2500 };
int HELIOS::SHOOT_TIME_S{ 1000 };
int HELIOS::SHOOT_TIME_E{ 4250 };
int HELIOS::SHOOT_SPEED_N{ 10 };
int HELIOS::SHOOT_SPEED_S{ 14 };

/**
	Just a helper function for getting the current time in milliseconds.

	@return uint64_t current time in milliseconds
*/
const uint64_t currentTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}