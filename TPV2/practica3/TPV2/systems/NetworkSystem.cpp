#include "NetworkSystem.h"

#include <SDL_net.h>

#include "../ecs/Manager.h"

#include "GameManagerSystem.h"
#include "FighterSystem.h"
#include "BulletsSystem.h"

NetworkSystem::NetworkSystem(const char *host, Uint16 port,
		std::string playerName) :
		host_(host), //
		port_(port), //
		isMaster_(false), //
		isGameReady_(false), //
		id_(0), //
		conn_(), //
		p_(nullptr), //
		m_(nullptr), //
		otherPlayerAddress_(), //
		localPlayerName_(playerName), //
		remotePlayerName_("N/A"), //
		lastTimeActive_(0) {
}

NetworkSystem::~NetworkSystem() {

	//
	if (conn_) {
		DisconnectMsg *m = static_cast<DisconnectMsg*>(m_);
		m->_type = _DISCONNECTED_;
		m->id = id_;
		p_->len = sizeof(DisconnectMsg);
		p_->address = otherPlayerAddress_;
		SDLNet_UDP_Send(conn_, -1, p_);
	}

	// close the connection
	if (conn_)
		SDLNet_UDP_Close(conn_);

	// free the packet
	if (p_) {
		SDLNet_FreePacket(p_);
	}

	// close SDLNet
	SDLNet_Quit();
}

void NetworkSystem::init() {

	// Initialise SDLNet
	if (SDLNet_Init() < 0) {
		throw SDLNet_GetError();
	}

	// allocate the packet to be used for sending data
	p_ = SDLNet_AllocPacket(100);
	m_ = reinterpret_cast<NetworkMessage*>(p_->data);

	if (host_ == nullptr) { // if started as a master player

		// we use id 0, and open a socket to send/receive messages
		isMaster_ = true;
		id_ = 0;
		conn_ = SDLNet_UDP_Open(port_);
		if (!conn_)
			throw SDLNet_GetError();

		names_[0] = localPlayerName_;
		names_[1] = remotePlayerName_;

	} else { // if started as  other player

		// we use id 1, and open a socket to send/receive messages
		isMaster_ = false;

		id_ = 1;
		conn_ = SDLNet_UDP_Open(0);
		if (!conn_)
			throw SDLNet_GetError();

		std::cout << "Trying to connect to other player at " << host_ << ":"
				<< port_ << std::endl;

		// resolve the host name into an IPAdrress
		if (SDLNet_ResolveHost(&otherPlayerAddress_, host_, port_) < 0) {
			throw SDLNet_GetError();
		}

		// send a message asking to play
		PlayRequestMsg *m = static_cast<PlayRequestMsg*>(m_);
		m->_type = _I_WANT_TO_PLAY_;
		memset(m->name, 0, 10);
		auto size =
				localPlayerName_.length() > 9 ? 9 : localPlayerName_.length();
		memcpy(m->name, localPlayerName_.c_str(), size);
		p_->len = sizeof(PlayRequestMsg);
		p_->address = otherPlayerAddress_;
		SDLNet_UDP_Send(conn_, -1, p_);

		// we use socket set to allow waiting for response instead of just looping
		SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
		SDLNet_UDP_AddSocket(socketSet, conn_);

		// wait for response
		if (SDLNet_CheckSockets(socketSet, 3000)) {
			if (SDLNet_SocketReady(conn_)) {
				if (SDLNet_UDP_Recv(conn_, p_) > 0) {
					if (m_->_type == _WELCOME_) {
						isGameReady_ = true;
						WelcomeMsg *m = static_cast<WelcomeMsg*>(m_);
						remotePlayerName_ = std::string(
								reinterpret_cast<char*>(m->name));
						id_ = m->id;
						names_[id_] = localPlayerName_;
						names_[1 - id_] = remotePlayerName_;
					}
				}
			}
		}

		// free the socket set, won't be used anymore
		SDLNet_FreeSocketSet(socketSet);

		// if did not succeed to connect, throw an exception
		if (!isGameReady_)
			throw "Failed to connect!";

	}
}

void NetworkSystem::update() {

	m_->_type = _ACTIVE_;
	p_->len = sizeof(NetworkMessage);
	p_->address = otherPlayerAddress_;
	SDLNet_UDP_Send(conn_, -1, p_);

	// in each iteration we poll all pending message and process them
	while (SDLNet_UDP_Recv(conn_, p_) > 0) {
		lastTimeActive_ = SDL_GetTicks();
		switch (m_->_type) {

		case _I_WANT_TO_PLAY_: {
			// we accept the connection if the player is the master, and no other player is connected
			if (isMaster_ && !isGameReady_) {
				PlayRequestMsg *m = static_cast<PlayRequestMsg*>(m_);
				otherPlayerAddress_ = p_->address;
				remotePlayerName_ = std::string(reinterpret_cast<char*>(m->name));
				names_[1 - id_] = remotePlayerName_;
				WelcomeMsg *mr = static_cast<WelcomeMsg*>(m_);

				mr->_type = _WELCOME_;
				auto size =
						localPlayerName_.size() > 9 ?
								9 : localPlayerName_.size();
				memcpy(mr->name, localPlayerName_.c_str(), size);
				mr->id = 1 - id_;
				p_->len = sizeof(WelcomeMsg);
				SDLNet_UDP_Send(conn_, -1, p_);
				isGameReady_ = true;
			}
			break;
		}

			// comienza la partida
		case _START_GAME_REQUEST_: {
			if (isMaster_) {
				manager_->getSystem<GameManagerSystem>()->startGame();
			}
			break;
		}

			// se actualiza puntuaciones y estado
		case _STATE_CHANGED_: {
			StateChangedMessage *m = static_cast<StateChangedMessage*>(m_);
			manager_->getSystem<GameManagerSystem>()->changeState(m->state_,
					m->left_score_, m->right_score_);
			break;
		}

			// se actualiza posicion del caza de la otra ventana
		case _FIGHTER_MOV_: {
			FighterMovementMsg *m = static_cast<FighterMovementMsg*>(m_);
			Vector2D pos(m->x, m->y);
			manager_->getSystem<FighterSystem>()->setPositionFighter(m->id, pos, m->rot);
			break;
		}
			// el otro usuario dispara una bala
		case _SHOOT_: {
			ShootMessage* m = static_cast<ShootMessage*>(m_);
			manager_->getSystem<BulletsSystem>()->shoot(m->id);
			break;
		}
			// el otro usuario produce una colision
		case _COLLIDES_: {
			StateChangedMessage* m = static_cast<StateChangedMessage*>(m_);
			// Se tiene que actualizar la puntacion antes porque en caso contrario ocurre un error con las puntuaciones
			// tras mandarse un mensaje en resetFighterPosition(). Lo que ocurre es que las puntuaciones deja de corresponderse
			// con lo que viene en el mensaje
			manager_->getSystem<GameManagerSystem>()->changeState(m->state_, m->left_score_, m->right_score_);
			manager_->getSystem<BulletsSystem>()->resetBullets();
			manager_->getSystem<FighterSystem>()->resetFighterPosition();
			sdlutils().soundEffects().at("explosion").play();
			break;
		}
			// el otro usuario se desconecta (se envia desde la destructora de NetworkSystem)
		case _DISCONNECTED_: {
			DisconnectMsg *m = static_cast<DisconnectMsg*>(m_);
			isGameReady_ = false;
			names_[m->id] = remotePlayerName_ = "N/A";
			manager_->getSystem<GameManagerSystem>()->resetGame();
			if (!isMaster_) {
				SDLNet_UDP_Close(conn_);
				conn_ = SDLNet_UDP_Open(port_);
				isMaster_ = true;
			}
			break;
		}
		}
	}

	if (isGameReady_ && SDL_GetTicks() - lastTimeActive_ > 3000) {
		isGameReady_ = false;
		names_[1 - id_] = remotePlayerName_ = "N/A";
		manager_->getSystem<GameManagerSystem>()->resetGame();
		if (!isMaster_) {
			SDLNet_UDP_Close(conn_);
			conn_ = SDLNet_UDP_Open(port_);
			isMaster_ = true;
		}
	}
}

void NetworkSystem::sendStartGameRequest() {
	m_->_type = _START_GAME_REQUEST_;
	p_->len = sizeof(NetworkMessage);
	p_->address = otherPlayerAddress_;
	SDLNet_UDP_Send(conn_, -1, p_);
}

void NetworkSystem::sendStateChanged(Uint8 state, Uint8 left_score,
		Uint8 right_score) {
	// if the other player is not connected do nothing
	if (!isGameReady_)
		return;

	// we prepare a message that includes all information
	StateChangedMessage *m = static_cast<StateChangedMessage*>(m_);
	m->_type = _STATE_CHANGED_;
	m->state_ = state;
	m->left_score_ = left_score;
	m->right_score_ = right_score;

	// set the message length and the address of the other player
	p_->len = sizeof(StateChangedMessage);
	p_->address = otherPlayerAddress_;

	// send the message
	SDLNet_UDP_Send(conn_, -1, p_);
}

void NetworkSystem::sendFighterMovement(Vector2D pos, float rot) {

	// if the other player is not connected do nothing
	if (!isGameReady_)
		return;

	// we prepare a message that includes all information
	FighterMovementMsg *m = static_cast<FighterMovementMsg*>(m_);
	m->_type = _FIGHTER_MOV_;
	m->x = pos.getX();
	m->y = pos.getY();
	m->rot = rot;
	m->id = id_;

	// set the message length and the address of the other player
	p_->len = sizeof(FighterMovementMsg);
	p_->address = otherPlayerAddress_;

	// send the message
	SDLNet_UDP_Send(conn_, -1, p_);
}

void NetworkSystem::sendShoot()
{
	// if the other player is not connected do nothing
	if (!isGameReady_)
		return;

	// we prepare a message that includes all information
	ShootMessage* m = static_cast<ShootMessage*>(m_);
	m->_type = _SHOOT_;
	m->id = id_;

	// set the message length and the address of the other player
	p_->len = sizeof(ShootMessage);
	p_->address = otherPlayerAddress_;

	// send the message
	SDLNet_UDP_Send(conn_, -1, p_);
}

void NetworkSystem::sendCollision(Uint8 state, Uint8 left_score, Uint8 right_score)
{
	// if the other player is not connected do nothing
	if (!isGameReady_)
		return;

	// we prepare a message that includes all information
	StateChangedMessage* m = static_cast<StateChangedMessage*>(m_);
	m->_type = _COLLIDES_;
	m->state_ = state;
	m->left_score_ = left_score;
	m->right_score_ = right_score;

	// set the message length and the address of the other player
	p_->len = sizeof(StateChangedMessage);
	p_->address = otherPlayerAddress_;

	// send the message
	SDLNet_UDP_Send(conn_, -1, p_);
}
