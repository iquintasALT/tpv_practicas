#pragma once

class Entity;

#include "../utils/Vector2D.h"

enum MsgId : Uint8 {
	LOSE_LIFE,
	RESET_ASTEROIDS,
	SHOOT_BULLET,
	BULLET_COLLIDES,
	START_GAME,
	WINNING
};

struct CollisionData {
	Entity* bullet;
	Entity* asteroid;

	~CollisionData() {};
	CollisionData(const CollisionData& cData) {
		bullet = cData.bullet;
		asteroid = cData.asteroid;
	}
	CollisionData() : bullet(), asteroid() {};
};

struct BulletData {
	Vector2D pos; 
	Vector2D vel; 
	double rot; 
	double width; 
	double height;

	BulletData() : pos(), vel(), rot(0), width(0), height(0) {};
	BulletData(const BulletData& bData) {
		pos = bData.pos;
		vel = bData.vel;
		rot = bData.rot;
		width = bData.width;
		height = bData.height;
	}
	~BulletData() {};
};

struct Message {
	MsgId id_;
	// El union es una forma de permitir que una clase tenga parametros que no ocupan memoria si no se inicializan
	// en caso de que se inicialice (con un msg.bMsg = si) para poder acceder despues habria que hacer casteo estatico
	// static_cast<&ElTipo>(msg);
	union { // uneme esta crack
		BulletData bData;
		CollisionData cData;
	};

	Message(MsgId id) : id_(id) {};
	Message(const Message& msg) {
		id_ = msg.id_;
		bData = msg.bData;
	};
	~Message() {};
};