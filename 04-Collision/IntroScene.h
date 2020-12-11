#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "Scene.h"
#include "TextureManager.h"
#include "GameSound.h"
#include "Sprite.h"
#include "Font.h"
#include "Simon.h"
#include "Board.h"
#include "Brick.h"
#include "HeliCopter.h"
#include "SceneIntroBat.h"

#include "MainScene.h"

#define INTRO_SCENE_STATUS_HANDLE_MENU 0 // Trạng thái khi đang xử lý ở phần ấn nút để start game
#define INTRO_SCENE_STATUS_HANDLE_GO_SCENE1 1 // Trạng thái xử lý khi đang ở phần Simon tự đi đến cửa lâu đài

#define INTRO_SCENE_SIMON_WALKING_SPEED 0.08f // Vận tốc di chuyển của Simon ở scene intro

class IntroScene : public Scene
{
	Sprite* MainMenuSprite;
	Sprite* IntroBatMenuSprite;
	GameSound* gameSound;
	Font PressStartText;

	/*Xử lí nhấn phím thì tiếp tục*/
	bool isPressedStart;
	DWORD WaitedTime; // ĐẾm thời gian đã nhấp nháy chữ

	/* Xử lí nhấp nháy TextPressStart */
	bool isAllowDrawTextPressStart;
	DWORD WaitedTimeToChangeDisplayTextAfterPressedStart; // Thay đổi trạng thái hiển thị chữ sau khi ấn nút

	int HandleStatus; // Trạng thái hiện tại đang xử lý

	Simon* simon;
	Board* board;
	Sprite* IntroGoScene1Sprite;
	Camera* camera;

	vector<LPGAMEOBJECT> listBricks;

	Helicopter* helicopter;
	SceneIntroBat* sceneIntroBat1;
	SceneIntroBat* sceneIntroBat2;

public:
	IntroScene();
	virtual ~IntroScene();

	void KeyState(BYTE* KeyState);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);

	void LoadResources();
	void Update(DWORD dt);
	void Render();
};

#endif
