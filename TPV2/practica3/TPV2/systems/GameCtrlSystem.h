#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../ecs/System.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

enum class GameState { NEWGAME, PAUSED, RUNNING, GAMEOVER, WIN };

/// <summary>
/// Sistema que gestiona la transicion de estados de juego
/// </summary>
class GameCtrlSystem : public System {
public:
	void update() override;

	//void render() override;

	//void receive(const Message& msg) override;

private:
	GameState gameState = GameState::NEWGAME;
	
	void onFighterDeath();
	void onAsteroidsExtinction();
};