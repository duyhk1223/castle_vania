#ifndef _DEFINE_H__
#define _DEFINE_H__


#include <fstream> 
#include <string>
#include <stdio.h>
#include <Windows.h>  
#include <iostream>
#include <map>
#include "debug.h" 
#include <algorithm>
#include <d3dx9.h>
#include <stdlib.h>
#include <iomanip>
#include <d3d9.h>
#include <vector>
#include <iostream>

using namespace std;

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 480 

#define MAX_FRAME_RATE 90

extern D3DCOLOR D3DBACKGROUND_COLOR; // Màu nền game
#define BACKGROUND_DEFAULT_COLOR D3DCOLOR_XRGB(0, 0, 0) // Màu đen

extern int isDebug_RenderBBox;
#define IS_DEBUG_RENDER_BBOX isDebug_RenderBBox

extern int isDebug_Untouchable;

// ID của Sprite, object
enum TAG
{
	SIMON = 01,
	SIMON_TRANS = 02,
	SIMON_DEADTH = 03,
	BBOX = 04,

	MAP1 = 1000,
	MAP2 = 2000,

	FONT = 101,
	BOARD = 102,
	BOARDHEALTH = 103,
	BOARD_TRANS = 107,

	// Brick
	BRICK = 21, // id object

	
	TEX_BRICK_MAP_2 = 23,
	TRANSPARENT_BRICK = 25,
	GATE = 26,
	TEX_BRICK_3_32 = 27,
	TEX_BRICK_4_32 = 28,

	// Object
	TORCH = 41,
	OBJECT_HIDDEN = 42,
	CANDLE = 43,
	STAIR_BOTTOM = 47,
	STAIR_TOP = 48,


	// Weapon
	MORNINGSTAR = 61,
	DAGGER = 62,
	HOLYWATER = 63,
	STOPWATCH = 64,
	FIREBALL = 65,
	THROWINGAXE = 66,
	BOOMERANG = 68,
	WEAPON_DOUBLE_SHOT = 69,
	NO_SUBWEAPON = 70,

	// Item
	LARGEHEART = 81,
	UPGRADEMORNINGSTAR = 82,
	ITEMDAGGER = 83,
	BONUS = 84,
	SMALLHEART = 85,
	ITEMHOLYWATER = 86,
	POTROAST = 87,
	CRYSTALBALL = 88,
	ITEMTHROWINGAXE = 89,
	INVISIBILITYPOTION = 90,
	CROSS = 91,
	ITEMBOOMERANG = 92,
	ITEMDOUBLESHOT = 93,
	MONEY_RED_BAG = 94,
	MONEY_WHITE_BAG = 95,
	MONEY_PURPLE_BAG = 96,

	// Effect
	HIT = 121,
	FIRE = 122,
	BROKENBRICK = 123,
	STEAM = 124,
	EFFECT_MONEY_700 = 125,
	EFFECT_MONEY_100 = 126,
	EFFECT_MONEY_400 = 127,
	EFFECT_MONEY_1000 = 128,

	// Enemy
	GHOST = 141,
	PANTHER = 142,
	BAT = 143,
	FISHMEN = 144,

	// Boss
	PHANTOMBAT = 161,

	// Scene Intro
	MAINMENU = 181, // Menu ở phần ấn phím để start game
	INTRO_GO_SCENE1 = 182, // Hình lâu đài
	INTRO_BAT_MENU = 183, // Bat ở menu (Phần press key to start)
	HELICOPTER = 184,
	INTRO_BAT = 185 // Bat ở intro (Phần Simon tự đi sau khi ấn phím)
};

#ifndef CHECK_OBJECT_IN_CAMERA 
#define CHECK_OBJECT_IN_CAMERA(obj) \
checkObjectInCamera(obj->GetX(), obj->GetY(), (float)obj->GetWidth(),  (float)obj->GetHeight())
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) \
if(ptr) \
{\
	delete (ptr); \
	ptr = nullptr; \
} \

#endif 

#endif