#ifndef __SCENEGAME_H__
#define __SCENEGAME_H__


#include "Scene.h"
#include "Camera.h"
#include "Sprite.h"

#include "Map.h"
#include "Grid.h"

#include "Brick.h"

class SceneGame : public Scene
{
private:
	Map* tileMap;
	Camera* camera;
	Grid* gridGame;

	TAG mapCurrent;
	vector<GameObject*> listObj;
public:
	SceneGame();
	~SceneGame();

	void KeyState(BYTE* state);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	void LoadResources();

	void InitGame(); // khởi tạo lại như chơi từ đầu
	void ResetResource(); // reset lai resource khi simon mất 1 mạng

	void Update(DWORD dt);
	void Render();


	void LoadMap(TAG);
	int StageCurrent;
};

#endif