#pragma once

#include "network_types.h"

#pragma pack(push,1)

enum MsgType : uint8_nt {
	_NONE_ = 0, //
	_I_WANT_TO_PLAY_, //
	_WELCOME_, //
	_PADDLE_POS, //
	_START_GAME_REQUEST_,
	_STATE_CHANGED_, //
	_DISCONNECTED_, //
	_ACTIVE_, //
	_SHOOT_
};

struct NetworkMessage {
	uint8_nt _type;
};

struct DissConnectMsg: NetworkMessage {
	uint8_nt id;
};


struct PlayRequestMsg: NetworkMessage {
	uint8_nt name[10];
};

struct WelcomeMsg: NetworkMessage {
	uint8_nt id;
	uint8_nt name[10];
};

struct PaddlePositionMsg: NetworkMessage {
	uint8_nt id;
	float32_nt x;
	float32_nt y;
};

struct StateChangedMessage: NetworkMessage {
	uint8_nt state_;
	uint8_nt left_score_;
	uint8_nt right_score_;
};

struct ShootMessage : NetworkMessage {
	uint8_nt id;
};
#pragma pack(pop)