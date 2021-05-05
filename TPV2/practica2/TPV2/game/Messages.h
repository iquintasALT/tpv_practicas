#pragma once

enum MsgId : Uint8 {
	HAHA,
	XD,
	LOL,
	LMAO,
	GOTTEM
};

struct BulletMsg {
	int x;
};

struct Message {
	MsgId id_;

	// El union es una forma de permitir que una clase tenga parametros que no ocupan memoria si no se inicializan
	// en caso de que se inicialice (con un msg.bMsg = tumadre) para poder acceder despues habria que hacer casteo estatico
	// static_cast<&ElTipo>(msg);
	union { // uneme esta crack
		BulletMsg bMsg;
	};

	Message(MsgId id) : id_(id) {};
	~Message() {};
};