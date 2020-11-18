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


#define GAME_TIME_MAX 300


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

	Item* DropItem(int Id, TAG Type, float X, float Y);

	void CheckCollision();

	int StageCurrent;
};

#endif