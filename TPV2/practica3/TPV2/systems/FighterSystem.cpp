//#include "FighterSystem.h"
//
//#include "../components/Image.h"
//#include "../components/Transform.h"
//#include "../components/Health.h"
//
//void FighterSystem::onCollisionWithAsteroid() {
//	// recolocamos el caza y reseteamos su velocidad y rotacion
//	player_tr_->pos_.set(sdlutils().width() / 2.0f - (player_tr_->width_ / 2),
//		sdlutils().height() / 2.0f - (player_tr_->height_ / 2));
//	player_tr_->vel_.set(Vector2D(0, 0));
//	player_tr_->rotation_ = 0;
//
//	// sonido de choque
//	crash_sfx_->play();
//}
//
//void FighterSystem::fighterToroidalMove()
//{
//	//toroidal en el eje X (fluido)
//	if (player_tr_->pos_.getX() > sdlutils().width())
//		player_tr_->pos_.setX(0 - player_tr_->width_);
//	else if (player_tr_->pos_.getX() + player_tr_->width_ < 0)
//		player_tr_->pos_.setX(sdlutils().width());
//
//	//toroidal en el eje Y (fluido)
//	if (player_tr_->pos_.getY() > sdlutils().height())
//		player_tr_->pos_.setY(0 - player_tr_->height_);
//	else if (player_tr_->pos_.getY() + player_tr_->height_ < 0)
//		player_tr_->pos_.setY(sdlutils().height());
//}
//
//void FighterSystem::init() {
//	fighter_ = manager_->addEntity();
//	player_tr_ = manager_->addComponent<Transform>(fighter_, Vector2D(sdlutils().width() / 2.0f - 25.0f,
//		sdlutils().height() / 2.0f - 25.0f), Vector2D(), 50.0f, 50.0f);
//	manager_->addComponent<Health>(fighter_, &sdlutils().images().at("heart"));
//	manager_->addComponent <Image>(fighter_, &sdlutils().images().at("fighter"));
//	manager_->setHandler<Player_hdlr>(fighter_);
//
//	thrust_sfx_ = &sdlutils().soundEffects().at("thrust");
//	crash_sfx_ = &sdlutils().soundEffects().at("explosion");
//}
//
//void FighterSystem::update() {
//	if (isRunning) {
//		if (ih().keyDownEvent()) {
//			if (ih().isKeyDown(SDL_SCANCODE_UP)) { // impulso
//				auto& vel = player_tr_->vel_;
//				auto newVel = vel + (Vector2D(0, -1).rotate(player_tr_->rotation_)).normalize() * thrust;
//				// si se pasa del limite de veocidad, establecemos este como nueva velocidad
//				vel.set((newVel.magnitude() > speedLimit) ?
//					(Vector2D(0, -1).rotate(player_tr_->rotation_)).normalize() * speedLimit : newVel);
//
//				thrust_sfx_->play(); // sonido de impulso
//			}
//			else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) { // rotacion izquierda
//				player_tr_->rotation_ = player_tr_->rotation_ - 5.0f;
//			}
//			else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) { // rotacion derecha
//				player_tr_->rotation_ = player_tr_->rotation_ + 5.0f;
//			}
//
//			if (ih().isKeyDown(SDLK_s) && sdlutils().currRealTime() - msToNextBullet > nextBullet) { // shoot
//
//				Message msg = Message(MsgId::SHOOT_BULLET);
//				msg.bData.pos = player_tr_->pos_; msg.bData.vel = player_tr_->vel_;
//				msg.bData.rot = player_tr_->rotation_; msg.bData.width = player_tr_->width_;
//				msg.bData.height = player_tr_->height_;
//				manager_->send(msg);
//
//				msToNextBullet = sdlutils().currRealTime();
//			}
//		}
//
//		player_tr_->pos_ = player_tr_->pos_ + player_tr_->vel_;
//		player_tr_->vel_ = player_tr_->vel_ * deAcceleration;
//
//		fighterToroidalMove();
//	}
//}
//
//void FighterSystem::render()
//{
//	//CAZA
//	SDL_Rect dest = build_sdlrect(player_tr_->pos_, player_tr_->width_, player_tr_->height_);
//	manager_->getComponent<Image>(fighter_)->tex_->render(dest, player_tr_->rotation_);
//
//	//VIDAS
//	Health* health = manager_->getComponent<Health>(fighter_);
//	Texture* tex = &sdlutils().images().at("heart");
//	Vector2D pos = Vector2D(0, 0);
//	for (int i = 0; i < health->lifes_; i++) {
//		pos.setX((health->heartSize + 5) * i); // dejamos un pequeño margen entre corazones 
//		SDL_Rect dest = build_sdlrect(pos, health->heartSize, health->heartSize);
//		tex->render(dest, 0);
//	}
//}
//
//void FighterSystem::receive(const Message& msg)
//{
//	switch (msg.id_) {
//	case MsgId::LOSE_LIFE:
//		onCollisionWithAsteroid();
//		break;
//	case MsgId::START_GAME:
//		isRunning = true;
//		msToNextBullet = sdlutils().currRealTime();
//		break;
//	case MsgId::STOP_RUNNING:
//	case MsgId::WINNING:
//		isRunning = false;
//		break;
//	}
//}