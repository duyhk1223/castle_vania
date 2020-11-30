#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"
#include "Camera.h"
#include "Sprite.h"
#include "Brick.h"
#include "Simon.h"
#include "Map.h"
#include "Grid.h"
#include "Item.h"
#include "Effect.h"
#include "BrokenBrick.h"
#include "Board.h"
#include "GameObject.h"
#include "Dagger.h"
#include "TextureManager.h"
#include "Bonus.h"
#include "UpgradeMorningStar.h"
#include "ItemDagger.h"
#include "LargeHeart.h"
#include "Hit.h"
#include "Fire.h"
#include "MoneyEffect.h"
#include "MoneyBag.h"
#include "SmallHeart.h"


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
};

#endif