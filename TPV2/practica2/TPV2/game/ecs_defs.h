// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class Image;
class Health;
class Generations;
class Follow;

#define _CMPS_LIST_  \
	Transform,\
	Image,\
	Health, \
	Generations, \
	Follow


// groups
struct Asteroid_grp;
struct Bullet_grp;
#define _GRPS_LIST_  Asteroid_grp, \
	Bullet_grp

// handlers
struct Player_hdlr;
#define _HDLRS_LIST_ Player_hdlr

// systems
struct GameCtrlSystem;
struct AsteroidsSystem;
struct BulletsSystem;
struct FighterSystem;
struct CollisionSystem;

#define _SYS_LIST_ GameCtrlSystem, \
	AsteroidsSystem, \
	BulletsSystem, \
	FighterSystem, \
	CollisionSystem
