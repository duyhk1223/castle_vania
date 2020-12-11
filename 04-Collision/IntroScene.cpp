#include "IntroScene.h"

IntroScene::IntroScene()
{
	LoadResources();
}

IntroScene::~IntroScene()
{
	SAFE_DELETE(simon);
	SAFE_DELETE(board);
}

void IntroScene::KeyState(BYTE* KeyState)
{
}

void IntroScene::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_2) // Lấy tọa độ world của chuột 
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(Game::GetInstance()->GetWindowHandle(), &p);
		DebugOut(L"[MOUSE POSITION] %d %d \n", p.x, p.y);
	}
	else
	{
		isPressedStart = true; // Nhấn phím khác bất kì thì xác nhận đã nhấn
	}
}

void IntroScene::OnKeyUp(int KeyCode)
{
}

void IntroScene::LoadResources()
{
	TextureManager* _textureManager = TextureManager::GetInstance(); // Đã load toàn bộ texture
	gameSound = GameSound::GetInstance(); // Đã load toàn bộ game sound

	MainMenuSprite = new Sprite(_textureManager->GetTexture(TAG::MAINMENU), 0);
	IntroBatMenuSprite = new Sprite(_textureManager->GetTexture(TAG::INTRO_BAT_MENU), 100);
	IntroGoScene1Sprite = new Sprite(_textureManager->GetTexture(TAG::INTRO_GO_SCENE1), 0);

	HandleStatus = INTRO_SCENE_STATUS_HANDLE_MENU;
	WaitedTime = 0;
	isPressedStart = 0;

	isAllowDrawTextPressStart = true;

	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->SetPosition(0, 0);

	simon = new Simon(camera);
	board = new Board(BOARD_DEFAULT_POSITION_X, BOARD_DEFAULT_POSITION_Y);

	helicopter = new Helicopter(432, 146);
	sceneIntroBat1 = new SceneIntroBat(297, 129, -0.01f, 0.0f); // Đi ngang qua trái
	sceneIntroBat2 = new SceneIntroBat(47, 224, 0.02f, -0.01f); // Đi góc dưới trái lên phải trên
}

void IntroScene::Update(DWORD dt)
{
	switch (HandleStatus)
	{
	case INTRO_SCENE_STATUS_HANDLE_MENU:
	{
		if (!isPressedStart) // Chưa nhấn phím để start game thì update sprite
			IntroBatMenuSprite->Update(dt);

		if (isPressedStart) // Nếu đã ấn nút để start game
		{
			WaitedTime += dt; // Đếm thời gian đợi để chuyển trạng thái xử lý
			if (WaitedTime >= 1000)
			{ // Chuyển qua trạng thái 2
				HandleStatus = INTRO_SCENE_STATUS_HANDLE_GO_SCENE1;
				gameSound->Play(Sound::musicGame_Start_Game_IntroScene);

				listBricks.push_back(new Brick(0, 400, 600, 32, BRICK_MODEL_TRANSPARENT));
				simon->SetPosition(500, 336);
				simon->SetDirection(-1); // Đi từ trái qua phải

				simon->SetAutoGoX(-1, -1, abs(225 - simon->GetX()), INTRO_SCENE_SIMON_WALKING_SPEED);
			}
			else
			{
				WaitedTimeToChangeDisplayTextAfterPressedStart += dt; // Đếm thời gian đã chớp của chữ
				if (WaitedTimeToChangeDisplayTextAfterPressedStart >= 150)
				{
					WaitedTimeToChangeDisplayTextAfterPressedStart = 0;
					isAllowDrawTextPressStart = !isAllowDrawTextPressStart; // Đảo trạng thái để ko render ra text nữa
				}
			}
		}

		break;
	}

	case INTRO_SCENE_STATUS_HANDLE_GO_SCENE1:
	{
		if (simon->GetIsAutoGoX())
		{
			simon->Update(dt, &listBricks); // Update Simon
		}
		else
		{
			simon->GetSprite()->SelectFrame(SIMON_ANI_HIDE_FACE);
		}

		helicopter->Update(dt);

		sceneIntroBat1->Update(dt);
		sceneIntroBat2->Update(dt);

		if (gameSound->GetIsSoundPlaying(Sound::musicGame_Start_Game_IntroScene) == false) // Sau khi Simon hoàn thành quá trình tự đi thì chuyển scene
		{
			SceneManager::GetInstance()->SetScene(new MainScene());
		}

		break;
	}
	}
}

void IntroScene::Render()
{
	 // Xét trạng thái đang xử lý
	switch (HandleStatus)
	{
	case INTRO_SCENE_STATUS_HANDLE_MENU:
	{
		MainMenuSprite->Draw(0, -3); // Vẽ hình main menu
		IntroBatMenuSprite->Draw(345, 195); // Vẽ intro bat ở màn hình chờ ấn nút start game
		if (isAllowDrawTextPressStart)
			PressStartText.Draw(160, 270, "PUSH START KEY");
		break;
	}

	case INTRO_SCENE_STATUS_HANDLE_GO_SCENE1:
	{

		IntroGoScene1Sprite->Draw(0, 0);

		helicopter->Render(camera);

		sceneIntroBat1->Render(camera);
		sceneIntroBat2->Render(camera);

		board->Render(simon, 1, 300, NULL);
		simon->Render(camera);

		break;
	}


	}
}