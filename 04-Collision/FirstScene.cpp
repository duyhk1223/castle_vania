#include "FirstScene.h"

FirstScene::FirstScene()
{
	LoadResources();
}

FirstScene::~FirstScene()
{
	SAFE_DELETE(firstSceneTileMap);
	SAFE_DELETE(mapGrid);
}

void FirstScene::KeyState(BYTE* state)
{
	if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
		firstSceneCamera->SetCameraPosition(firstSceneCamera->GetCamCoordinateX() + 2, firstSceneCamera->GetCamCoordinateY());
	if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
		firstSceneCamera->SetCameraPosition(firstSceneCamera->GetCamCoordinateX() - 2, firstSceneCamera->GetCamCoordinateY());
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
	mapGrid = new Grid();

	firstSceneCamera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	InitGamePlayForFirstScene();
}

void FirstScene::InitGamePlayForFirstScene()
{
	LoadFirstSceneMap(objectType::MAP1);
}

void FirstScene::ResetGameResources()
{
	mapGrid->ReloadMapGrid(); // nạp lại grid
}

void FirstScene::Update(DWORD dt)
{
	mapGrid->GetListObjectFromMapGrid(mapObjectsList, firstSceneCamera);
	firstSceneCamera->Update(dt);

	for (UINT i = 0; i < mapObjectsList.size(); i++)
		mapObjectsList[i]->Update(dt, &mapObjectsList);  // đã kiểm tra "Alive" lúc lấy từ lưới ra
}

void FirstScene::Render()
{
	// Nếu game vẫn còn chưa kết thúc
	firstSceneTileMap->DrawMap(firstSceneCamera);

	for (UINT i = 0; i < mapObjectsList.size(); i++)
		mapObjectsList[i]->Render(firstSceneCamera);

	//if (!isGameOver)
	//{
	//	
	//}
	//else
	//{
	//	// Sau khi kết thúc game
	//}
	
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
		mapGrid->SetObjectFilePath("Resources/map/file_gameobject_map1.txt");
		firstSceneTileMap->LoadMapFromResource(objectType::MAP1);

		
		// Set up camera với Simon
		firstSceneCamera->SetIsFollowingSimon(true);

		firstSceneCamera->SetCameraBoundary(0.0f, (float)(firstSceneTileMap->GetMapWidth() - firstSceneCamera->GetCamWidth())); // set biên camera dựa vào kích thước map
		//camera->SetBoundaryBackup(camera->GetBoundaryLeft(), camera->GetBoundaryRight()); // backup lại biên

		firstSceneCamera->SetCameraPosition(0, 0);
		//------------------------------------------------//

		currentState = 1; // Khởi tạo màn hiện tại
		break;

	case objectType::MAP2:
		mapGrid->SetObjectFilePath("Resources/map/file_gameobject_map2.txt");
		firstSceneTileMap->LoadMapFromResource(mapType);

		// Set up camera với Simon
		//------------------------------------------------//
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