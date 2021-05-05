#include "AsteroidsSystem.h"

#include "../systems/GameCtrlSystem.h"

#include "../components/Generations.h"
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../components/Follow.h"

void AsteroidsSystem::addAsteroid()
{
	auto asteroid = manager_->addEntity();

	int lives = sdlutils().rand().nextInt(1, 4); //vidas aleatorias

	//marco de aparicion
	Vector2D c = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2) +
		Vector2D(sdlutils().rand().nextInt(-100, 101), sdlutils().rand().nextInt(-100, 101));
	//se escogem la anchura o la altura aleatoriamente
	int selection = sdlutils().rand().nextInt(0, 2);
	//tras escoger una de ellas, la segunda componente del vector sera la que no se ha escogido en primer lugar
	//(quitamos el ancho/alto del asteroide para que spawneen siempre dentro 
	int first = (selection == 0 ? sdlutils().rand().nextInt(0, sdlutils().width() - (20 + 10 * lives)) :
		sdlutils().rand().nextInt(0, 2) * (sdlutils().width() - (20 + 10 * lives)));
	int second = (selection == 0 ? sdlutils().rand().nextInt(0, 2) * (sdlutils().height() - (20 + 10 * lives)) :
		sdlutils().rand().nextInt(0, sdlutils().height() - (20 + 10 * lives)));

	//creamos el vector pos y vel
	Vector2D pos = Vector2D(first, second);
	Vector2D vel = (c - pos).normalize() * (sdlutils().rand().nextInt(1, 10) / 10.0);

	// 20 + 10 * lives para el tama�o dependiente de las generaciones que le quedan
	manager_->addComponent<Transform>(asteroid, pos, vel, 20 + 10 * lives, 20 + 10 * lives, 0.0f);
	manager_->addComponent<Generations>(asteroid, lives);

	// 70% TIPO A, 30% TIPO B
	bool followPlayer = false;
	if (sdlutils().rand().nextInt() % 100 < 30) {
		followPlayer = true;
		//IMAGEN 
		manager_->addComponent<Image>(asteroid, &sdlutils().images().at("asteroid_gold"), 5, 6);
	}
	else manager_->addComponent<Image>(asteroid, &sdlutils().images().at("asteroid"), 5, 6);
	manager_->addComponent<Follow>(asteroid, followPlayer);
	//se añade al grupo para comprobar colisiones mas tarde
	manager_->setGroup<Asteroid_grp>(asteroid, true);
	numOfAsteroids_++;
}

void AsteroidsSystem::addAsteroid(int gen, Entity* prev_asteroid)
{
	auto asteroid = manager_->addEntity();

	// rotacion aleatoria
	int rand = sdlutils().rand().nextInt(0, 360);
	Transform* t = manager_->getComponent<Transform>(prev_asteroid);

	// creamos el vector pos y vel
	Vector2D pos = t->pos_ + t->vel_.rotate(rand) * 2 * t->width_;
	Vector2D vel = t->vel_.rotate(rand) * 1.1f;

	// 20 + 10 * lives para el tamaño dependiente de las generaciones que le quedan
	manager_->addComponent<Transform>(asteroid, pos,
		vel, 20 + 10 * gen, 20 + 10 * gen, 0.0f);
	manager_->addComponent<Generations>(asteroid, gen);

	// 70% TIPO A, 30% TIPO B
	bool followPlayer = false;
	if (sdlutils().rand().nextInt() % 100 < 30) {
		followPlayer = true;
		//IMAGEN
		manager_->addComponent<Image>(asteroid, &sdlutils().images().at("asteroid_gold"), 5, 6);
	}
	else manager_->addComponent<Image>(asteroid, &sdlutils().images().at("asteroid"), 5, 6); //IMAGEN

	manager_->addComponent<Follow>(asteroid, followPlayer);
	//se añade al grupo para comprobar colisiones mas tarde
	manager_->setGroup<Asteroid_grp>(asteroid, true);
	numOfAsteroids_++;
	numOfAsteroids_++;
}

void AsteroidsSystem::addAsteroids(int n)
{
	for (int i = 0; i < n; i++) addAsteroid();
}

void AsteroidsSystem::onCollisionWithBullet(Entity* hit_asteroid, Entity* bullet)
{
	int gen = --manager_->getComponent<Generations>(hit_asteroid)->gen_;
	manager_->setActive(hit_asteroid, false);

	if (gen > 0) {
		addAsteroid(gen, hit_asteroid);
		addAsteroid(gen, hit_asteroid);
	}

	numOfAsteroids_--;
	//si ya no quedan asteroides, el jugador ha ganado
	if (numOfAsteroids_ <= 0) {
		// reseteamos la posicion del jugador, y eliminamos los componentes FighterCtrl y Gun
		auto fighter = manager_->getHandler<Player_hdlr>();
		auto fighterTr_ = manager_->getComponent<Transform>(fighter);

		fighterTr_->pos_.set(sdlutils().width() / 2.0f - (fighterTr_->width_ / 2),
			sdlutils().height() / 2.0f - (fighterTr_->height_ / 2));
		fighterTr_->vel_.set(Vector2D(0, 0));
		fighterTr_->rotation_ = 0;

		manager_->getSystem<GameCtrlSystem>()->setGameState(GameState::WIN);
	}
}

void AsteroidsSystem::asteroidOppositeSide(Entity* asteroid)
{
	Transform* tr = manager_->getComponent<Transform>(asteroid);
	//toroidal en el eje X (fluido)
	if (tr->pos_.getX() > sdlutils().width())
		tr->pos_.setX(0 - tr->width_);
	else if (tr->pos_.getX() + tr->width_ < 0)
		tr->pos_.setX(sdlutils().width());

	//toroidal en el eje Y (fluido)
	if (tr->pos_.getY() > sdlutils().height())
		tr->pos_.setY(0 - tr->height_);
	else if (tr->pos_.getY() + tr->height_ < 0)
		tr->pos_.setY(sdlutils().height());
}

void AsteroidsSystem::asteroidFollow(Entity* asteroid)
{
	Transform* fighter_tr_ = manager_->getComponent<Transform>(manager_->getHandler<Player_hdlr>());
	Transform* tr_ = manager_->getComponent<Transform>(asteroid);
	auto& vel = tr_->vel_;
	// rotamos progresivamente a la izquierda o derecha dependiendo
	// de si el caza esta a su derecha o a su izquierda
	vel = vel.rotate(vel.angle(fighter_tr_->pos_ - tr_->pos_) > 0 ? 1.0f : -1.0f);
}

void AsteroidsSystem::update()
{
	//COMPORTAMIENTO DE CADA ASTEROIDE
	for (Entity* asteroid : manager_->getEntities()) {
		if (manager_->hasGroup<Asteroid_grp>(asteroid)) {
			if (manager_->getComponent<Follow>(asteroid)->followsPlayer) asteroidFollow(asteroid);
			asteroidOppositeSide(asteroid);
			Transform* tr = manager_->getComponent<Transform>(asteroid);
			tr->pos_ = tr->pos_ + tr->vel_;
		}
	}
	//TIMER PARA NUEVO ASTEROIDE
	if (manager_->getSystem<GameCtrlSystem>()->getGameState() == GameState::RUNNING && sdlutils().currRealTime() - msToNextAsteroid > newAsteroidSpawn) {
		addAsteroid();
		msToNextAsteroid = sdlutils().currRealTime();
	}
}

void AsteroidsSystem::render()
{
	if (manager_->getSystem<GameCtrlSystem>()->getGameState() == GameState::RUNNING) {
		for (Entity* e : manager_->getEntities()) {
			if (manager_->hasGroup<Asteroid_grp>(e)) {
				Image* image = manager_->getComponent<Image>(e);
				Transform* tr_ = manager_->getComponent<Transform>(e);

				//render por frames
				if (sdlutils().currRealTime() - image->lastUpdate > ms) {
					if (image->cols_ == image->actCol_ + 1) { // si ha llegado al final de la fila (nCols)
						// actualizamos la fila en caso de que sea el final o no
						image->actRow_ = (image->rows_ == image->actRow_ + 1 ? 0 : image->actRow_ + 1);
						image->actCol_ = 0; // reseteamos columna
					}
					else image->actCol_++;

					// actualizamos timer
					image->lastUpdate = sdlutils().currRealTime();
				}

				SDL_Rect src = { image->actCol_ * (image->tex_->width() / image->cols_),
								image->actRow_ * (image->tex_->height() / image->rows_),
								image->tex_->width() / image->cols_ ,
								image->tex_->height() / image->rows_ };

				SDL_Rect dest = build_sdlrect(tr_->pos_, tr_->width_, tr_->height_);
				image->tex_->render(src, dest, tr_->rotation_);
			}
		}
	}

}
