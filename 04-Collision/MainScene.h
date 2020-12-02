#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

// Others
#include "Scene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Brick.h"
#include "Simon.h"
#include "Map.h"
#include "Grid.h"
#include "Board.h"
#include "GameObject.h"
#include "TextureManager.h"

// Effect
#include "Effect.h"
#include "Hit.h"
#include "Fire.h"
#include "MoneyEffect.h"
#include "BrokenBrick.h"

 // Item
#include "Item.h"
#include "Bonus.h"
#include "ItemDagger.h"
#include "MoneyBag.h"
#include "SmallHeart.h"
#include "PotRoast.h"
#include "ItemDoubleShot.h"
#include "ItemHolyWater.h"
#include "ItemThrowingAxe.h"
#include "LargeHeart.h"
#include "UpgradeMorningStar.h"
#include "InvisibilityPotion.h"
#include "Cross.h"
#include "ItemBoomerang.h"
#include "ItemStopWatch.h"

// Weapon
#include "Dagger.h"


#define GAME_TIME_MAX 300

#define CAMERA_BOUNDARY_BEFORE_GO_GATE1_RIGHT (2576.0f-15.0f) // Biên phải camera trước khi qua cửa 1


// Toạ độ camera trc và sau khi qua của 1
#define GATE1_POSITION_CAM_BEFORE_GO 2809.0f
#define GATE1_POSITION_CAM_AFTER_GO 3073.0f

// Toạ độ camera trc và sau khi qua cửa 2
#define GATE2_POSITION_CAM_BEFORE_GO 3840.0f
#define GATE2_POSITION_CAM_AFTER_GO 4095.0f

#define DISTANCE_AUTO_WALK_AFTER_GATE 80.0f // Simon phải tự đi 80px sau khi chạm vào cửa



#define CAMERA_POSITION_Y_LAKE 374.0f // Toạ độ camera khi Simon xuống hồ nước

// Biên giới hạn phần hồ nước
#define CAMERA_BOUNDARY_LAKE_LEFT 3075.0f
#define CAMERA_BOUNDARY_LAKE_RIGHT (4111.0f-SCREEN_WIDTH)


#define CAMERA_BOUNDARY_BOSS_RIGHT (5648.0f - SCREEN_WIDTH) // Biên camera cho phần boss


#define INVISIBILITYPOTION_LIMITTIMEWAIT 4000 // Thời gian sử dụng InvisibilityPotion

#define CROSS_MAXUSINGTIME 1000 // Thời gian tối đa khi dùng Cross


class MainScene : public Scene
{
private:
	Map* tileMap;
	Camera* camera;
	Grid* gridGame;
	Board* board;

	Simon* simon;

	TAG mapCurrent;
	vector<GameObject*> listObj;
	vector<Item*> listItem;
	vector<Effect*> listEffect;

	Sprite* _spriteLagerHeart;

	/*Xử lí đi qua cửa 1*/
	bool isHandlingGoThroughTheDoor1;
	bool isWentThroughTheDoor1;

	/*Xử lí đi qua cửa 2*/
	bool isHandlingGoThroughTheDoor2;
	bool isWentThroughTheDoor2;

	/*Xử lí liên quan InvisibilityPotion*/
	bool isUsingInvisibilityPotion;
	DWORD InvisibilityPotion_WaitedTime; // Đếm thòi gian Simon đã tàng hình

	/*Xử lí Cross*/
	bool isUsingCross;
	DWORD Cross_WaitedTime; // Thời gian đã sử dụng Cross
	DWORD Cross_ChangeColorBackground_WaitedTime; // Thời gian đã qua khi thay đổi màu nền
	DWORD Cross_ChangeColorBackground_MaxWaitingTime; // Thời gian cần chờ để đổi màu nền

public:
	MainScene();
	~MainScene();

	void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void LoadResources();

	void InitGame(); // Khởi tạo lại như chơi từ đầu
	void ResetResource(); // Reset lại resource khi Simon mất 1 mạng

	void Update(DWORD dt);
	void Render();

	void LoadMap(TAG mapType);

	

	void CheckCollisionWeapon(vector<GameObject*> listObj);
	void CheckCollisionSimonItem();
	void CheckCollisionSimonAndHiddenObject();
	void CheckCollisionSimonWithGate();

	Item* DropItem(int Id, TAG Type, float X, float Y);

	void CheckCollision();

	int StageCurrent;

	void HandleInvisibilityPotion(DWORD dt); // Xử lý khi Simon nhặt dc thuốc tàng hình
	void HandleCross(DWORD dt); // Xử lý khi Simon nhặt dc thánh giá 
};

#endif