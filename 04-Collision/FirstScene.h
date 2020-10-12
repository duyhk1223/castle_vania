#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__

#include "Scene.h"
#include "Camera.h"
#include "GSprite.h"
#include "Map.h"
#include "Simon.h"
#include "Grid.h"
#include "TextureManager.h"
#include "GameObject.h"

class FirstScene : public Scene
{
private:
	Map* firstSceneTileMap;
	Camera* firstSceneCamera; 

	vector<GameObject*> mapObjectsList;
	vector<GameObject*> enemyList;

	/* Xử lí gameover*/
	bool isGameOver;

	objectType currentMap; // Biến lưu trữ giá trị biểu thị map hiện tại

public:
	FirstScene();
	~FirstScene();

	void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void LoadResources();

	void Update(DWORD dt);
	void Render();

	void InitGamePlayForFirstScene(); // Hàm khổi tạo game để chơi từ đầu
	void ResetGameResources(); // Reset game resource khi Simon mất 1 mạng

	void LoadFirstSceneMap(objectType mapType);
};

#endif