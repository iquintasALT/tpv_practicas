#include "GameManager.h"

void GameManager::render()
{
	//si no esta jugando
	if (states_ != states::RUNNING) {
		
		if (states_ == states::GAMEOVER) {
			auto& t = sdlutils().msgs().at("gameover");
			t.render((sdlutils().width() - t.width()) / 2,
				(sdlutils().height() - t.height()) / 2);
		}

		if (states_ == NEWGAME) {
			auto& t = sdlutils().msgs().at("start");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}
		else {
			auto& t = sdlutils().msgs().at("continue");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}
	}
}

void GameManager::update()
{
	if (ih().isKeyDown(SDL_SCANCODE_C)) {
		states_ = (states)(((int)states_ + 1 )% 4);
	}
}
