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
	// Nếu đang ko tấn công thì mới ngồi dc
	if (Game::GetInstance()->IsKeyDown(DIK_DOWN) && simon->isAttacking == false)
	{
		simon->Sit();
		if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
			simon->Right();

		if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
			simon->Left();
		return;
	}
	
	if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
	{
		simon->Right();
		simon->Go();
	}
	else
		if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
		{
			simon->Left();
			simon->Go();
		}
		else
		{
			simon->Stop();
		}

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


	if (!(Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) /*&& simon->isProcessingOnStair == 0*/ && simon->isAttacking == true))
		if (KeyCode == DIK_A /*&& simon->isProcessingOnStair == 0*/) // Không phải đang xử lí việc đi trên thang thì đc đánh
		{
			simon->SetSpeed(0, 0);
			simon->Attack(TAG::MORNINGSTAR);
		}

	if (simon->isJumping && simon->isWalking)
	{
		return;
	}


	if (KeyCode == DIK_S /*&& simon->isOnStair == false*/)
	{
		if (Game::GetInstance()->IsKeyDown(DIK_LEFT) || Game::GetInstance()->IsKeyDown(DIK_RIGHT))
		{
			simon->Stop();
			simon->SetSpeed(SIMON_WALKING_SPEED * simon->GetDirection(), -SIMON_VJUMP);
			simon->isJumping = 1;
			simon->isWalking = 1;
		}
		else
		{
			simon->Jump();
		}
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
	simon = new Simon(camera);

	InitGame();
}

void FirstScene::InitGame()
{
	LoadMap(TAG::MAP1);
	simon->Init();
}

void FirstScene::ResetResource()
{
	gridGame->ReloadGrid();
}

void FirstScene::Update(DWORD dt)
{
	gridGame->GetListObject(listObj, camera);


	simon->Update(dt, &listObj);

	if (camera->AllowFollowSimon())
		camera->SetPosition(simon->GetX() - SCREEN_WIDTH / 2 + 30, camera->GetYCam()); // cho camera chạy theo simon

	camera->Update(dt);



	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);

}

void FirstScene::Render()
{
	tileMap->DrawMap(camera);

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Render(camera);

	simon->Render(camera);
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

void FirstScene::LoadMap(TAG mapType)
{
	mapCurrent = mapType;

	switch (mapType)
	{
	case TAG::MAP1:
		gridGame->SetFilePath("Resources/map/file_gameobject_map1.txt");

		tileMap->LoadMap(TAG::MAP1);

		camera->SetAllowFollowSimon(true);

		camera->SetBoundary(0.0f, (float)(tileMap->GetMapWidth() - camera->GetWidth())); // set biên camera dựa vào kích thước map
		

		camera->SetPosition(0, 0);

		simon->SetPosition(SIMON_DEFAULT_POSITION);
		
		break;
	}

	ResetResource();
}

#pragma region Phần cài đặt cho các hàm xử lý va chạm

#pragma endregion

#pragma region Các hàm xử lý cho phần process (Item, clear state,...)

#pragma endregion
