#include "FirstScene.h"

FirstScene::FirstScene()
{
	LoadGameResources();
}

FirstScene::~FirstScene()
{
	SAFE_DELETE(firstSceneTileMap);
}

void FirstScene::KeyState(BYTE* state)
{
	if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
	{

	}
	else
	{

	}
}

void FirstScene::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_R) // render bbox
	{
		if (isDebug_RenderBBox == 0)
			isDebug_RenderBBox = 1;
		else
			isDebug_RenderBBox = 0;
	}

	if (KeyCode == DIK_ESCAPE)
		DestroyWindow(Game::GetInstance()->GetWindowHandle()); // Thoát game
}

void FirstScene::OnKeyUp(int KeyCode)
{

}

void FirstScene::LoadResources()
{
	TextureManager* _textureManager = TextureManager::GetInstance(); // Load toàn bộ các hình ảnh lên
	firstSceneTileMap = new Map();

	firstSceneCamera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	InitGamePlayForFirstScene();
}

void FirstScene::InitGamePlayForFirstScene()
{
	LoadFirstSceneMap(objectType::MAP1);
}

void FirstScene::ResetGameResources()
{

}

void FirstScene::Update(DWORD dt)
{
	
}

void FirstScene::Render()
{
	if (!isGameOver)
	{
		firstSceneTileMap->DrawMap(firstSceneCamera);
	}
	
	// Vẽ các object trong grid

	// Vẽ các item

	// Vẽ enemy

	// Vẽ Simon

	// Vẽ các effect

	// Vẽ các weapon của enemy

	// Vẽ boss (nếu có)
}

void FirstScene::LoadFirstSceneMap(objectType mapType)
{
	currentMap = mapType;

	switch (mapType)
	{
	case objectType::MAP1:
		firstSceneTileMap->LoadMapFromResource(mapType);

		// Set up camera với Simon
		break;

	case objectType::MAP2:
		firstSceneTileMap->LoadMapFromResource(mapType);

		// Set up camera với Simon
		break;
	default:
		break;
	}

	ResetGameResources();
}

#pragma region Phần cài đặt cho các hàm xử lý va chạm

#pragma endregion

#pragma region Các hàm xử lý cho phần process (Item, clear state,...)

#pragma endregion