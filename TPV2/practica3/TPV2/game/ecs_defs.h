#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
struct Transform;
struct Image;
struct FighterCtrlKeys;

#define _CMPS_LIST_ \
	Transform,\
	Image,\
	FighterCtrlKeys

// groups
struct LeftBullet;
struct RightBullet;
#define _GRPS_LIST_ \
	LeftBullet, \
	RightBullet

// handlers
struct LeftFighter;
struct RightFighter;
#define _HDLRS_LIST_ \
	LeftFighter, \
	RightFighter

// systems
class FighterSystem;
class RenderSystem;
class BulletsSystem;
class CollisionSystem;
class GameManagerSystem;
class NetworkSystem;
#define _SYS_LIST_ \
		FighterSystem, \
		RenderSystem, \
        BulletsSystem, \
		CollisionSystem, \
		GameManagerSystem,\
		NetworkSystem
