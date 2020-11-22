#include "MainScene.h"

MainScene::MainScene()
{
	LoadResources();
}

MainScene::~MainScene()
{
}

void MainScene::KeyState(BYTE* state)
{

	if (simon->GetFreeze() == true) // disable control
	{
		return;
	}

	if (Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && !simon->isAttacking)
	{
		simon->Attack(simon->GetTypeSubWeapon()); // attack với vũ khí phụ đang nhặt
	}

	//ngồi
	if (Game::GetInstance()->IsKeyDown(DIK_DOWN) && simon->isAttacking == false && simon->isJumping == false )
	{
		simon->Sit();
		
		if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
			simon->Right();

		if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
			simon->Left();
		return;
	}

	if (simon->isJumping && simon->isWalking)
	{
		return;
	}

	if (simon->isAttacking) // đang attack
	{
		float vx, vy;
		simon->GetSpeed(vx, vy);
		simon->SetSpeed(0, vy); // thì không thể đi

		return;
	}

	if (simon->isJumping == 0) {
		if (Game::GetInstance()->IsKeyDown(DIK_RIGHT) && simon->isSitting == 0)
		{
			simon->Right();
			simon->Go();
		}
		else
			if (Game::GetInstance()->IsKeyDown(DIK_LEFT) && simon->isSitting == 0)
			{
				simon->Left();
				simon->Go();
			}
			else
			{
				simon->Stop();
			}
		
	}
}

void MainScene::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_R) // render bbox
	{
		if (isDebug_RenderBBox == 0)
			isDebug_RenderBBox = 1;
		else
			isDebug_RenderBBox = 0;
	}


	if (!(Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && simon->isAttacking == true)) {
		if (KeyCode == DIK_A) // tấn công bằng roi
		{
			simon->Attack(TAG::MORNINGSTAR);
		}
	}
		
	if (simon->isJumping && simon->isWalking)
	{
		return;
	}


	if (KeyCode == DIK_S && simon->isJumping == 0) // nhảy
	{
		if (Game::GetInstance()->IsKeyDown(DIK_LEFT) || Game::GetInstance()->IsKeyDown(DIK_RIGHT)) // đang đi khi nhảy sẽ nhảy hết hình cung
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

void MainScene::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void MainScene::LoadResources()
{
	TextureManager* _textureManager = TextureManager::GetInstance();

	tileMap = new Map();
	gridGame = new Grid();
	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	simon = new Simon(camera);
	board = new Board(BOARD_DEFAULT_POSITION_X, BOARD_DEFAULT_POSITION_Y);

	_spriteLagerHeart = new Sprite(TextureManager::GetInstance()->GetTexture(TAG::LARGEHEART), 100);

	InitGame();
}

void MainScene::InitGame()
{
	LoadMap(TAG::MAP1);
	simon->Init();
}

void MainScene::ResetResource()
{
	gridGame->ReloadGrid();
}

void MainScene::Update(DWORD dt)
{
	

	// xử lí freeze
	if (simon->GetFreeze() == true)
	{
		simon->UpdateFreeze(dt);

		if (simon->GetFreeze() == true)// dang freeze thì k update
			return;
	}

	gridGame->GetListObject(listObj, camera);

	simon->Update(dt, &listObj);

	if (camera->AllowFollowSimon())
		camera->SetPosition(simon->GetX() - SCREEN_WIDTH / 2 + 30, camera->GetYCam()); // cho camera chạy theo simon

	camera->Update(dt);



	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);

	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Update(dt, &listObj);

	for (UINT i = 0; i < listEffect.size(); i++)
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Update(dt);

	CheckCollision();
}

void MainScene::Render()
{
	tileMap->DrawMap(camera);

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Render(camera);
	
	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Render(camera);

	for (UINT i = 0; i < listEffect.size(); i++)
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Render(camera);

	simon->Render(camera);

	board->Render(simon, StageCurrent, GAME_TIME_MAX, NULL); // Vẽ tạm
}

void MainScene::LoadMap(TAG mapType)
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

		StageCurrent = 1;
		
		break;
	}

	ResetResource();
}

void MainScene::CheckCollisionWeapon(vector<GameObject*> listObj) // kt va chạm của vũ khí 
{
	
	for (auto& objWeapon : simon->mapWeapon)
	{
		if (objWeapon.second->GetFinish() == false) // Vũ khí đang hoạt động
		{
			
			for (UINT i = 0; i < listObj.size(); i++) {
				if (objWeapon.second->GetLastTimeAttack() > listObj[i]->GetLastTimeAttacked()) // Nếu chưa xét va chạm của lượt attack này ở các frame trước
				{
					if (objWeapon.second->isCollision(listObj[i]) == true) // nếu có va chạm
					{
						
						bool RunEffectHit = false;
						GameObject* gameObj = listObj[i];
						switch (gameObj->GetType()) {
						case TAG::TORCH:
							gameObj->SubHealth(1);
							listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
							RunEffectHit = true;
							break;
						}

						if (RunEffectHit)
						{
							listEffect.push_back(new Hit(listObj[i]->GetX() + 10, listObj[i]->GetY() + 14)); // hiệu ứng hit
							listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // hiệu ứng lửa

							
							// DAGGER chạm obj sẽ mất
							if (objWeapon.second->GetType() == TAG::DAGGER)
							{
								objWeapon.second->SetFinish(true);
							}

						}
					}
				}
			}
		}
	}

}

void MainScene::CheckCollisionSimonItem()
{
	for (UINT i = 0; i < listItem.size(); i++) 
	{
		if (listItem[i]->GetFinish() == false && listItem[i]->IsWaitingToDisplay() == false) // chưa kết thúc và "không phải" đang chờ để hiển thị
		{
			if (simon->isCollisionWithItem(listItem[i]) == true) // có va chạm
			{
				switch (listItem[i]->GetType())
				{
				case TAG::LARGEHEART:
					simon->SetHeartCollect(simon->GetHeartCollect() + 5);
					listItem[i]->SetFinish(true);

					break;



				case TAG::UPGRADEMORNINGSTAR:
				{
					MorningStar* objMorningStar = dynamic_cast<MorningStar*>(simon->mapWeapon[TAG::MORNINGSTAR]);
					objMorningStar->UpgradeLevel(); // Nâng cấp roi

					listItem[i]->SetFinish(true);
					simon->SetFreeze(true); // bật trạng thái đóng băng 

					break;
				}
					


				//Sub weapon item
				case TAG::ITEMDAGGER:
				{
					simon->PickUpSubWeapon(TAG::DAGGER);
					listItem[i]->SetFinish(true);
					break;
				}
					
				}
				
			}
		}
	}
}

void MainScene::CheckCollision()
{
	CheckCollisionWeapon(listObj); 
	CheckCollisionSimonItem();
}



Item* MainScene::DropItem(int Id, TAG Type, float X, float Y) // xử lí rớt item
{
	if (mapCurrent == TAG::MAP1)
	{
		if (Type == TAG::TORCH)
		{
			if (Id == 5 || Id == 4)
				return new LargeHeart(X, Y);

			if (Id == 2 || Id == 3)
				return new UpgradeMorningStar(X, Y);

			if (Id == 1)
				return new ItemDagger(X, Y);
		}

		if (Type == TAG::OBJECT_HIDDEN)
		{
			if (Id == 8)
				return new Bonus(1240, 305);
		}
	}
}

