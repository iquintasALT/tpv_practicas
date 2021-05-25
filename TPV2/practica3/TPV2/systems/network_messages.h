#pragma once

#include "network_types.h"

#pragma pack(push,1)

enum MsgType : uint8_nt {
	_NONE_ = 0,
	_I_WANT_TO_PLAY_,
	_START_GAME_REQUEST_,
	_STATE_CHANGED_,
	_FIGHTER_MOV_,
	_SHOOT_,
	_COLLIDES_,
	_RESET_FIGHTER_,
	_DISCONNECTED_,
	_WELCOME_,
	_ACTIVE_
};

struct NetworkMessage {
	uint8_nt _type;
};

struct DissConnectMsg : NetworkMessage {
	uint8_nt id;
};

struct PlayRequestMsg : NetworkMessage {
	uint8_nt name[10];
};

struct WelcomeMsg : NetworkMessage {
	uint8_nt id;
	uint8_nt name[10];
};

struct StateChangedMessage : NetworkMessage {
	uint8_nt state_;
	uint8_nt left_score_;
	uint8_nt right_score_;
};

struct FighterMovementMsg : NetworkMessage {
	uint8_nt id;
	float32_nt x;
	float32_nt y;
	float32_nt rot;
};

struct ShootMessage : NetworkMessage {
	uint8_nt id;
};

struct ResetMessage : NetworkMessage {
	uint8_nt id;
};
#pragma pack(pop)