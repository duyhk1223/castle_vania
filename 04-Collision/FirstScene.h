#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"
#include "Camera.h"
#include "Sprite.h"

#include "Simon.h"

#include "Map.h"
#include "Grid.h"

#include "TextureManager.h"
#include "GameObject.h"
#include "Simon.h"
#include "Brick.h"

class FirstScene : public Scene
{
private:
	Map* tileMap;
	Camera* camera;
	Grid* gridGame;

	Simon* simon;

	TAG mapCurrent;
	vector<GameObject*> listObj;

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