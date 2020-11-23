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

	if (Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) /*&& simon->isProcessingOnStair == 0*/ && !simon->isAttacking)
	{
		simon->Attack(simon->GetTypeSubWeapon()); // attack với vũ khí phụ đang nhặt
	}

	// Nếu đang ko tấn công thì mới ngồi dc
	if (Game::GetInstance()->IsKeyDown(DIK_DOWN) && simon->isAttacking == false && simon->isJumping == false)
	{
		simon->Sit();
		if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
			simon->Right();

		if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
			simon->Left();
		return;
	}
	
	if (simon->isAttacking && simon->isJumping)
		return;

	if (simon->isAttacking) // Nếu Simon đang attack thì không thể đi tiếp
	{
		float vx, vy;
		simon->GetSpeed(vx, vy);
		simon->SetSpeed(0, vy);

		return;
	}

	// Nếu đang nhảy thì ko thể đổi chiều Simon
	if (simon->isJumping)
		return;

	// Simon đang tấn công thì ko thể đi dc
	if (Game::GetInstance()->IsKeyDown(DIK_RIGHT) && simon->isSitting == false)
	{
		simon->Right();
		simon->Go();
	}
	else
		if (Game::GetInstance()->IsKeyDown(DIK_LEFT) && simon->isSitting == false)
		{
			simon->Left();
			simon->Go();
		}
		else
		{
			simon->Stop();
		}

}

void MainScene::OnKeyDown(int KeyCode)
{
	if (KeyCode == DIK_R) // render bbox debug
	{
		if (isDebug_RenderBBox == 0)
			isDebug_RenderBBox = 1;
		else
			isDebug_RenderBBox = 0;
	}


	if (!(Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && simon->isAttacking == true))
		if (KeyCode == DIK_A)
		{
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

void MainScene::OnKeyUp(int KeyCode)
{
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

	case TAG::MAP2:
			gridGame->SetFilePath("Resources/map/file_gameobject_map2.txt");
			tileMap->LoadMap(TAG::MAP2);

			camera->SetAllowFollowSimon(true);
			camera->SetPosition(0, 0);
			camera->SetBoundary(0, CAMERA_BOUNDARY_BEFORE_GO_GATE1_RIGHT); // biên camera khi chưa qua cửa
			
			simon->SetPosition(SIMON_DEFAULT_POSITION);
			break;
	}

	ResetResource();
}

#pragma region Các hàm check va chạm

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

						/* Vũ khí phụ của simon */
						case TAG::ITEMDAGGER:
						{
							simon->PickUpSubWeapon(TAG::DAGGER);
							listItem[i]->SetFinish(true);
							break;
						}


						}

						if (RunEffectHit)
						{
							listEffect.push_back(new Hit(listObj[i]->GetX() + 10, listObj[i]->GetY() + 14)); // hiệu ứng hit
							listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // hiệu ứng lửa

							
							// Nếu Dagger va chạm với object thì sẽ mất
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

void MainScene::CheckCollisionSimonAndHiddenObject()
{
	for (UINT i = 0; i < listObj.size(); i++)
	{
		if (listObj[i]->GetType() == TAG::OBJECT_HIDDEN)
		{
			GameObject* object = listObj[i];

			if (object->GetHealth() > 0)
			{
				if (simon->isCollitionObjectWithObject(object)) // Có va chạm với object xảy ra
				{
					if (mapCurrent == TAG::MAP1)
					{
						switch (object->GetId())
						{
						case 7: // Hiden object ở cửa
							LoadMap(TAG::MAP2);

							return;
						case 8: // Hiden object của bonus
							listItem.push_back(DropItem(object->GetId(), object->GetType(), simon->GetX(), simon->GetY()));
							break;
						}
						object->SubHealth(1);
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
	CheckCollisionSimonAndHiddenObject();
}

Item* MainScene::DropItem(int Id, TAG Type, float X, float Y) // xử lí rớt item
{
	if (mapCurrent == TAG::MAP1)
	{
		if (Type == TAG::TORCH)
		{
			if (Id == 1 || Id == 4)
				return new LargeHeart(X, Y);

			if (Id == 2 || Id == 3)
				return new UpgradeMorningStar(X, Y);

			if (Id == 5)
				return new ItemDagger(X, Y);
		}

		if (Type == TAG::OBJECT_HIDDEN)
		{
			if (Id == 8)
				return new Bonus(1240, 305);
		}
	}
}

#pragma endregion

