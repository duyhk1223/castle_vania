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
#include "Board.h"
#include "GameObject.h"
#include "Dagger.h"
#include "TextureManager.h"
#include "Bonus.h"



class FirstScene : public Scene
{
private:
	Map* tileMap;
	Camera* camera;
	Grid* gridGame;
	Board* board;

	Simon* simon;

	TAG mapCurrent;
	vector<GameObject*> listObj;

	Sprite* _spriteLagerHeart;

public:
	FirstScene();
	~FirstScene();

	void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void LoadResources();

	void InitGame(); // Khởi tạo lại như chơi từ đầu
	void ResetResource(); // Reset lại resource khi Simon mất 1 mạng

	void Update(DWORD dt);
	void Render();


	void LoadMap(TAG mapType);
	int StageCurrent;
};

#endif