#ifndef SHOOTER_CONSTANTS
#define SHOOTER_CONSTANTS

#include<chrono>

// constants for screen size
const int WIDTH{ 120 };
const int HEIGHT{ 30 };

const int REQUIRED_SCORE{ 125 };

// Stuff for balancing/stats
const struct PLAYER {
	const static int HEALTH{ 20 };
	const static int H_SPEED{ 10 };
	const static int V_SPEED{ 7 };
	static int SHOOT_TIME;
	static int SHOOT_SPEED;
	const static double SLIDE;
};

const struct ASTEROID {
	const static int HEALTH{ 5 };
	const static int SPEED{ 5 };
};

const struct BASICSHIP {
	const static int HEALTH{ 3 };
	const static int SPEED{ 4 };
	const static int SHOOT_TIME{ 2500 };
	const static int SHOOT_SPEED{ 9 };
};

const struct ROCKET {
	const static int HEALTH{ 3 };
	const static int SPEED{ 10 };
};

const struct TRISHOOTER {
	const static int HEALTH{ 4 };
	const static int SPEED{ 3 };
	const static int SHOOT_TIME{ 3250 };
	const static int SHOOT_SPEED{ 8 };
};

const struct SNIPER {
	const static int HEALTH{ 5 };
	const static int SPEED{ 2 };
	const static int SHOOT_TIME{ 4000 };
	const static int SHOOT_SPEED{ 18 };
};

const struct CHASER {
	const static int HEALTH{ 3 };
	const static int SPEED{ 12 };
};

const struct EXPLODER {
	const static int HEALTH{ 3 };
	const static int SPEED{ 8 };
	const static int PIECE_SPEED{ 8 };
};

const struct HELIOS {
	const static int HEALTH{ 110 };
	static int CHASE_SPEED;
	const static int HAND_HEALTH{ 25 };
	const static int SHIELD_HEALTH{ 35 };
	static int SHOOT_TIME_N;
	static int SHOOT_SPEED_N;
	static int SHOOT_TIME_S;
	static int SHOOT_SPEED_S;
	static int SHOOT_TIME_E;
};

const double PI = atan(1) * 4;

// List of possible entities
const enum class EntityType {
	PLAYER,
	PLAYER_BULLET,
	ENEMY,
	ENEMY_BULLET,
	BOSS
};

const uint64_t currentTime();

#endif