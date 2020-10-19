#include "SceneGame.h"

SceneGame::SceneGame()
{
	LoadResources();
}

SceneGame::~SceneGame()
{
}

void SceneGame::KeyState(BYTE* state)
{
}

void SceneGame::OnKeyDown(int KeyCode)
{
}

void SceneGame::OnKeyUp(int KeyCode)
{
}

void SceneGame::LoadResources()
{
	TextureManager* _textureManager = TextureManager::GetInstance();

	tileMap = new Map();
	gridGame = new Grid();
	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	InitGame();
}

void SceneGame::InitGame()
{
	LoadMap(TAG::MAP1);
}

void SceneGame::ResetResource()
{
	
}

void SceneGame::Update(DWORD dt)
{
	gridGame->GetListObject(listObj, camera);


	camera->Update(dt);
}

void SceneGame::Render()
{
	tileMap->DrawMap(camera);

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Render(camera);
}

void SceneGame::LoadMap(TAG x)
{
	mapCurrent = x;

	switch (x)
	{
	case TAG::MAP1:
		gridGame->SetFilePath("Resources/map/file_gameobject_map1.txt");
		tileMap->LoadMap(TAG::MAP1);

		camera->SetAllowFollowSimon(true);

		camera->SetBoundary(0.0f, (float)(tileMap->GetMapWidth() - camera->GetWidth())); // set biên camera dựa vào kích thước map
		//camera->SetBoundaryBackup(camera->GetBoundaryLeft(), camera->GetBoundaryRight()); // backup lại biên

		camera->SetPosition(0, 0);

		camera->SetAllowFollowSimon(true);

		

		StageCurrent = 1;
		break;
	}

	ResetResource();
}
