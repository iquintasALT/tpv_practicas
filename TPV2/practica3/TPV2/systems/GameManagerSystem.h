#pragma once
#include <SDL_stdinc.h>
#include <array>

#include "../ecs/System.h"

struct Transform;

class GameManagerSystem : public System {
public:
	enum GameState : Uint8 {
		NEWGAME,   // just before starting a new game
		PAUSED,    // between rounds
		RUNNING,   // playing
		GAMEOVER   // game over
	};

	enum FighterType : Uint8 {
		LEFT_FIGHTER,
		RIGHT_FIGHTER
	};

	GameManagerSystem();
	virtual ~GameManagerSystem();

	Uint8 getState() { return state_; }
	const auto& getScore() { return score_; }

	void init() override;
	void update() override;
	
	void startGame();
	void changeState(Uint8 state, Uint8 left_score, Uint8 right_score);
	void onCollision(Uint8 id);
	void resetGame();

private:
	std::array<Uint8, 2> score_;
	Uint8 state_;
	Uint8 maxScore_;
};

