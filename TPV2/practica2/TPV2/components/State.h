#pragma once

#include "../ecs/Component.h"

enum class GameState { NEWGAME, PAUSED, RUNNING, GAMEOVER, WIN };

class State : public Component {
	GameState state_;

	State() : state_(GameState::NEWGAME) {}
	virtual ~State() {}
};
