#pragma once

#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

#include "../components/State.h"
#include "../components/Generations.h"
#include "../components/FramedImage.h"
#include "../components/Follow.h"
#include "../components/Transform.h"

class AsteroidsManager : public Component {
public:
	AsteroidsManager() : numAsteroids(0), msToNextAsteroid(0), state_(nullptr) {}

	virtual ~AsteroidsManager() {}

	void init() override {
		state_ = entity_->getComponent<State>();

		assert(state_ != nullptr);
	}

	inline int getNumAsteroids() { return numAsteroids; }

	void update() override {
		if (state_->getState() == states::RUNNING && sdlutils().currRealTime() - msToNextAsteroid > 5000) {
			generateAsteroid();
			msToNextAsteroid = sdlutils().currRealTime();
		}
	}

	void generateAsteroid(int lives = 3, Entity* prev_asterroid = nullptr)
	{
		auto asteroid = entity_->getMngr()->addEntity();
		////componentes
		//si no se pasa una referencia a un asteroide, creo uno en una posición aleatoria
		Vector2D pos, vel;
		int rand = sdlutils().rand().nextInt(0, 360);
		//creacion de un asteroide a partir de otro
		if (prev_asterroid != nullptr) {
			Transform* t = prev_asterroid->getComponent<Transform>();

			pos = t->getPos() + t->getVel().rotate(rand) * 2 * t->getW();
			vel = t->getVel().rotate(rand) * 1.1f;
		}
		//creacion de un asteroide desde 0
		else {
			//pos centro
			Vector2D c = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) +
				Vector2D(sdlutils().rand().nextInt(-100, 101), sdlutils().rand().nextInt(-100, 101));
			//escogemos la anchura o la altura aleatoriamente
			int selection = sdlutils().rand().nextInt(0, 2);
			//tras escoger una de ellas, la segunda componente del vector sera la contraria
			int first = selection == 0 ? sdlutils().rand().nextInt(0, sdlutils().width()) : 
				sdlutils().rand().nextInt(0, 2) * sdlutils().width();
			int second = selection == 0 ? sdlutils().rand().nextInt(0, 2) * sdlutils().height() :
				sdlutils().rand().nextInt(0, sdlutils().height());
			//creamos el vector pos y vel
			pos = Vector2D(first, second);
			vel = (c - pos).normalize() * (sdlutils().rand().nextInt(1, 10) / 10.0);

		}

		asteroid->addComponent<Transform>(pos,
			vel, 20 + 10 * lives, 20 + 10 * lives, 0.0f);

		asteroid->addComponent<ShowAtOppositeSide>(sdlutils().width(), sdlutils().height());

		asteroid->addComponent<Generations>(lives);
		////70% TIPO A, 30% TIPO B
		if (sdlutils().rand().nextInt() % 100 < 30) {
			asteroid->addComponent<Follow>(entity_->getMngr()->getHandler<Player_hdlr>()->getComponent<Transform>());
			asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid_gold"), 5, 6);
		}
		else asteroid->addComponent<FramedImage>(&sdlutils().images().at("asteroid"), 5, 6);

		asteroid->setGroup<Asteroid_grp>(asteroid);
		numAsteroids++;
	}
	
	void onCollision(Entity* hit_asteroid) {
		int lives = --hit_asteroid->getComponent<Generations>()->getLives();
		hit_asteroid->setActive(false);

		if (lives > 0) {
			generateAsteroid(lives, hit_asteroid);
			generateAsteroid(lives, hit_asteroid);
		}

		numAsteroids--; 
		if (numAsteroids <= 0) state_->setState(states::WIN);
	}

private:
	int numAsteroids;
	int msToNextAsteroid;
	State* state_;
};
