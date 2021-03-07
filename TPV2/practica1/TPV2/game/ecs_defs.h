// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// components -- forward declaration, i.e., assume they are defined somewhere
class Transform;
class Rectangle;
class FighterCtrl;
class Bounce;
class Image;
class Rotate;
class DeAcceleration;
class Heart;
class ShowAtOppositeSide;
class DisableOnExit;
class Gun;

#define _CMPS_LIST_  \
	Transform,\
	Rectangle,\
	FighterCtrl,\
	Bounce,\
	Image,\
	Rotate,\
	DeAcceleration, \
	Heart, \
	ShowAtOppositeSide, \
	DisableOnExit, \
	Gun

// groups

struct Asteroid_grp;
struct Bullet_grp;
#define _GRPS_LIST_  Asteroid_grp, \
	Bullet_grp

// handlers
struct Hdlr_1;
#define _HDLRS_LIST_ Hdlr_1
