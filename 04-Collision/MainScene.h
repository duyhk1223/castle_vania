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
#include "GameTime.h"
#include "GameSound.h"

// Effect
#include "Effect.h"
#include "Hit.h"
#include "Fire.h"
#include "MoneyEffect.h"
#include "BrokenBrick.h"
#include "Steam.h"

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

// Enemy
#include "Ghost.h"
#include "BlackPanther.h"
#include "Bat.h"
#include "Fishmen.h"

// Boss
#include "PhantomBat.h"


#define MAX_GAME_TIME 300 // Thời gian chơi game



#pragma region Ghost

#define WAIT_TIME_BETWEEN_TWO_GHOST_IS_CREATED 1000 // 1 giây - Khoảng thời gian phải chờ giữa ghost đầu và ghost sẽ được tạo tiếp theo
#define WAIT_TIME_BEFORE_ALLOW_TO_CREATE_GHOST 2500 // 2.5 giây - Thời gian phải chờ trc khi dc tạo ghost

#define GHOST_ZONE1_LEFT -16.0f
#define GHOST_ZONE1_RIGHT 825.0f

#define GHOST_ZONE2_LEFT 2200.0f
#define GHOST_ZONE2_RIGHT 2775.0f

#define GHOST_ZONE3_LEFT 4233.0f  // Biên trái vùng hoạt động
#define GHOST_ZONE3_RIGHT 4993.0f // Biên phải vùng hoạt động
#define GHOST_ZONE3_COLUMN1 4412.0f 
#define GHOST_ZONE3_COLUMN2 4590.0f

#pragma endregion



#pragma region Black Panther

// 2 biên giới hạn cho vùng tạo báo
#define REGION_CREATE_PANTHER_BOUNDARY_LEFT 1090.0f
#define REGION_CREATE_PANTHER_BOUNDARY_RIGHT 2305.0f

#pragma endregion




#pragma region Fishmen

#define FISHMEN_ZONE_1_LEFT 3065.0f
#define FISHMEN_ZONE_1_RIGHT 3193.0f
#define FISHMEN_POS_1 3121.0f

#define FISHMEN_ZONE_2_LEFT 3193.0f
#define FISHMEN_ZONE_2_RIGHT 3326.0f
#define FISHMEN_POS_2 3254.0f

#define FISHMEN_ZONE_3_LEFT 3326.0f
#define FISHMEN_ZONE_3_RIGHT 3458.0f
#define FISHMEN_POS_3 3382.0f

#define FISHMEN_ZONE_4_LEFT 3458
#define FISHMEN_ZONE_4_RIGHT 3571
#define FISHMEN_POS_4 3505 

#define FISHMEN_ZONE_5_LEFT 3571.0f
#define FISHMEN_ZONE_5_RIGHT 3707.0f
#define FISHMEN_POS_5 3636.0f

#define FISHMEN_ZONE_6_LEFT 3707.0f
#define FISHMEN_ZONE_6_RIGHT 3829.0f
#define FISHMEN_POS_6 3760.0f

#define FISHMEN_ZONE_7_LEFT 3829.0f
#define FISHMEN_ZONE_7_RIGHT 3956.0f
#define FISHMEN_POS_7 3881.0f


#define FISHMEN_ZONE_8_LEFT 3956.0f
#define FISHMEN_ZONE_8_RIGHT 4095.0f
#define FISHMEN_POS_8 4017.0f

#define FISHMEN_POS_Y 805.0f


#pragma endregion




#pragma region Gate

#define CAMERA_BOUNDARY_BEFORE_GO_GATE1_RIGHT (2576.0f-15.0f) // Biên phải camera trước khi qua cửa 1

// Toạ độ camera trc và sau khi qua của 1
#define GATE1_POSITION_CAM_BEFORE_GO 2809.0f
#define GATE1_POSITION_CAM_AFTER_GO 3073.0f

// Toạ độ camera trc và sau khi qua cửa 2
#define GATE2_POSITION_CAM_BEFORE_GO 3840.0f
#define GATE2_POSITION_CAM_AFTER_GO 4095.0f

#define DISTANCE_AUTO_WALK_AFTER_GATE 80.0f // Simon phải tự đi 80px sau khi chạm vào cửa

#pragma endregion




#pragma region Lake

#define CAMERA_POSITION_Y_LAKE 374.0f // Toạ độ camera khi Simon xuống hồ nước

// Biên giới hạn phần hồ nước
#define CAMERA_BOUNDARY_LAKE_LEFT 3075.0f
#define CAMERA_BOUNDARY_LAKE_RIGHT (4111.0f - SCREEN_WIDTH)

#pragma endregion



#pragma region Bat

#define CREATE_BAT_BOUNDARY_DIVISION_DIRECTION_X 3590.0f // Biên chia đôi vùng, bên trái thì Bat bay hướng -1, phải thì 1, trước khi qua hồ cá và chưa lên thang qua cửa 2
#define CREATE_BAT_BOUNDARY_DIVISION_DIRECTION_Y 207.0f // Biên chia đôi vùng, trên thì bay hướng 1, dưới thì -1, trước cửa 2

#pragma endregion




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
	GameSound* gameSound;

	PhantomBat* phantomBat; // Boss

	Sprite* _spriteLagerHeart;

	GameTime* gameTime; //Game time

	TAG mapCurrent;
	vector<GameObject*> listObj;
	vector<Item*> listItem;
	vector<Effect*> listEffect;
	vector <GameObject*> listEnemy;

	vector <Weapon*> listWeaponOfEnemy; // List chứa các viên đạn của enemy



	/*Xử lí liên quan tạo ghost*/
	int CurrentGhostEnemyCount; // Số lượng ghost hiện tại
	DWORD CreateGhostTime; // Thời điểm bắt đầu tạo ghost
	DWORD BeginWaitingToCreateGhostTime; // Thời điểm bắt đầu chờ xử lí việc tạo ghost
	bool isWaitingToCreateGhost; // Chờ xử lí việc tạo ghost
	bool isAllowCheckTimeWaitToCreateGhost = false; // Cho phép kt thời gian chờ xử lí tạo ghost

	/*Xử lí liên quan tạo Panther*/
	bool isAllowCreatePanther;
	int CurrentPantherEnemyCount;

	/*Xử lí liên quan tạo Bat*/
	DWORD CreateBatTime; // Thời điểm bắt đầu tạo Bat
	DWORD WaitingTimeToCreateBat; // Thời gian phải chờ để tạo bot
	bool isAllowToCreateBat; // Biến cờ để cho phép tạo Bat hoặc ko
	int CurrentEnemyBatCount; // Đếm số lượng dơi hiện tại

	/*Xử lí liên quan tạo Fishmen*/
	bool isAllowToCreateFishmen;
	int CurrentFishmenEnemyCount;
	DWORD CreateFishmenTime; // Thời điểm đã tạo fishmen
	DWORD WaitingtimeToCreateFishmen; // Thời gian cần chờ để tạo fishmen

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
	void CheckCollisionWithEnemy(); // Check xem Simon có tấn công enemy hay bị thương khi va chạm ko
	void CheckCollisionSimonWithEnemy(); // Check va chạm giữa Simon và enemy
	void CheckCollisionWithBoss(); // Check va chạm giữa Simon và Boss


	Item* DropItem(int Id, TAG Type, float X, float Y);

	void CheckCollision();

	int StageCurrent;

	void HandleInvisibilityPotion(DWORD dt); // Xử lý khi Simon nhặt dc thuốc tàng hình
	void HandleCross(DWORD dt); // Xử lý khi Simon nhặt dc thánh giá 

	void PlayGameMusic(); // Hàm khởi tạo lại âm thanh game
};

#endif