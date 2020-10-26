#include "FirstScene.h"

FirstScene::FirstScene()
{
	LoadResources();
}

FirstScene::~FirstScene()
{
}

void FirstScene::KeyState(BYTE* state)
{
	//=========================================
	if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
		camera->SetPosition(camera->GetXCam() + 2, camera->GetYCam());
	if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
		camera->SetPosition(camera->GetXCam() - 2, camera->GetYCam());
}

void FirstScene::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_R) // render bbox debug
	{
		if (isDebug_RenderBBox == 0)
			isDebug_RenderBBox = 1;
		else
			isDebug_RenderBBox = 0;
	}



}

void FirstScene::OnKeyUp(int KeyCode)
{
}

void FirstScene::LoadResources()
{
	TextureManager* _textureManager = TextureManager::GetInstance();

	tileMap = new Map();
	gridGame = new Grid();
	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	InitGame();
}

void FirstScene::InitGame()
{
	LoadMap(objectType::MAP1);
}

void FirstScene::ResetResource()
{
	gridGame->ReloadGrid();
}

void FirstScene::Update(DWORD dt)
{
	gridGame->GetListObject(listObj, camera);

	camera->Update(dt);

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);



}

void FirstScene::Render()
{
	tileMap->DrawMap(camera);

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Render(camera);

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

void FirstScene::LoadMap(objectType x)
{
	mapCurrent = x;

	switch (x)
	{
	case objectType::MAP1:
		gridGame->SetFilePath("Resources/map/file_gameobject_map1.txt");

		tileMap->LoadMap(objectType::MAP1);

		camera->SetAllowFollowSimon(true);

		camera->SetBoundary(0.0f, (float)(tileMap->GetMapWidth() - camera->GetWidth())); // set biên camera dựa vào kích thước map
		//camera->SetBoundaryBackup(camera->GetBoundaryLeft(), camera->GetBoundaryRight()); // backup lại biên

		camera->SetPosition(0, 0);


		StageCurrent = 1;
		break;
	}

	ResetResource();
}

#pragma region Phần cài đặt cho các hàm xử lý va chạm

#pragma endregion

#pragma region Các hàm xử lý cho phần process (Item, clear state,...)

#pragma endregion
