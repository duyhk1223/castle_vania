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
		return;

	// Nếu Simon đang tự đi
	if (simon->GetIsAutoGoX() == true) // Đang chế độ tự đi thì ko xét phím
		return;

	if (camera->GetIsAutoGoX()) // Camera đang chế độ tự đi thì ko xét phím
		return;

	if (Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && simon->isProcessingOnStair == 0 && !simon->isAttacking)
	{
		simon->Attack(simon->GetTypeSubWeapon()); // attack với vũ khí phụ đang nhặt
	}
	else
		if (!simon->isJumping)
		{
			if (Game::GetInstance()->IsKeyDown(DIK_UP) && simon->isAttacking == false)
			{
				if (!simon->isOnStair) // Simon chưa trên thang
				{
					for (UINT i = 0; i < listObj.size(); i++)
						if (listObj[i]->GetType() == TAG::STAIR_BOTTOM)
						{
							if (simon->isCollisionAxisYWithBrick && simon->isCollitionObjectWithObject(listObj[i])) // nếu va chạm với trục y brick và STAIR BOTOM
							{
								GameObject* gameobj = dynamic_cast<GameObject*>(listObj[i]);
								simon->directionStair = gameobj->GetDirection(); // Lưu hướng của cầu thang đang đi vào simon
								simon->directionY = -1; // Hướng đi lên
								simon->SetDirection(simon->directionStair); // Hướng của simon khi đi lên là hướng của cầu thang

								simon->isOnStair = true; // Set trạng thái đang trên cầu thang
								simon->passedDistance = 0;

								// Trường hợp Simon va chạm 1 trong 2 bìa của StairTop
								if (simon->GetX() < gameobj->GetX())
								{
									// Biên phải
									simon->SetAutoGoX(1, gameobj->GetDirection(), gameobj->GetX() - simon->GetX(), SIMON_WALKING_SPEED);
									// Hướng sau khi autogo phải là hướng của cầu thang:  gameobj->GetDirection()
								}
								else // Biên trái
									simon->SetAutoGoX(-1, gameobj->GetDirection(), simon->GetX() - gameobj->GetX(), SIMON_WALKING_SPEED);
								//	DebugOut(L"bat dau len cau thang!\n"); 
								return;
							}
						}
				}
				else // Nếu Simon đã trên cầu thang
				{
					DebugOut(L"Da o tren cau thang!\n");

					if (simon->isProcessingOnStair == 0 || simon->isProcessingOnStair == 3) // Kết thúc xử lí trước đó
					{
						simon->isWalking = true;
						simon->isProcessingOnStair = 1;
						simon->directionY = -1; // Hướng đi lên
						simon->SetDirection(simon->directionStair); // Hướng của simon khi đi lên là hướng của cầu thang
						simon->SetSpeed(simon->GetDirection() * SIMON_SPEED_ONSTAIR, -1 * SIMON_SPEED_ONSTAIR);

						float vvx, vvy;
						simon->GetSpeed(vvx, vvy);
						//DebugOut(L"vy = %f\n", vvy);
					}
				}
			}
			else
			{
				if (Game::GetInstance()->IsKeyDown(DIK_DOWN) && simon->isAttacking == false) // Ngược lại nếu nhấn nút xuống
				{
					if (!simon->isOnStair) // Chưa trên cầu thang
					{
						int CountCollisionTop = 0;
						for (UINT i = 0; i < listObj.size(); i++)
							if (listObj[i]->GetType() == TAG::STAIR_TOP)
							{
								if (simon->isCollitionObjectWithObject(listObj[i])
									&&
									simon->isCollisionAxisYWithBrick
									) // Nếu va chạm với STAIR TOP
								{
									GameObject* gameobj = dynamic_cast<GameObject*>(listObj[i]);
									simon->directionStair = gameobj->GetDirection(); // lưu hướng của cầu thang đang đi vào simon
									simon->directionY = 1;// hướng đi xuống
									simon->SetDirection(simon->directionStair);// hướng của simon khi đi xuống là hướng của cầu thang

									simon->isOnStair = true; // set trạng thái đang trên cầu thang
									simon->passedDistance = 0;


									if (simon->GetX() < gameobj->GetX())
									{
										simon->SetAutoGoX(1, -gameobj->GetDirection(), gameobj->GetX() - simon->GetX(), SIMON_WALKING_SPEED);
										// hướng sau khi autogo phải là hướng của cầu thang:  gameobj->GetDirection()
									}
									else
										simon->SetAutoGoX(-1, -gameobj->GetDirection(), simon->GetX() - gameobj->GetX(), SIMON_WALKING_SPEED);



									CountCollisionTop++;
									return;
								}
							}

						if (CountCollisionTop == 0) // Ko đụng stair top, tức là ngồi bt
						{
							simon->Sit();
							if (Game::GetInstance()->IsKeyDown(DIK_RIGHT))
								simon->Right();

							if (Game::GetInstance()->IsKeyDown(DIK_LEFT))
								simon->Left();
							return;
						}

					}
					else // Đã ở trên cầu thang thì có thể đi xuống
					{
						if (simon->isProcessingOnStair == 0 || simon->isProcessingOnStair == 3) // Kết thúc xử lí trước đó
						{
							simon->isWalking = true;
							simon->isProcessingOnStair = 1;
							simon->directionY = 1; // Hướng đi xuống
							simon->SetDirection(simon->directionStair * -1); // Hướng của simon khi đi xuóng là ngược của cầu thang
							simon->SetSpeed(simon->GetDirection() * SIMON_SPEED_ONSTAIR, SIMON_SPEED_ONSTAIR);
						}

					}
				}
				else
				{
					simon->Stop();
				}
			}

		}

	if (simon->isJumping && simon->isWalking)
		return;

	if (simon->isOnStair) // Nếu đang trên thang thì không xét loại đi trái phải
		return;

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

	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}

	if (!(Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && simon->isAttacking == true))
		if (KeyCode == DIK_A)
		{
			simon->Attack(TAG::MORNINGSTAR);
		}

	if (simon->isJumping && simon->isWalking)
		return;


	if (KeyCode == DIK_S && simon->isOnStair == false)
	{
		if (simon->isJumping == false) // Nếu đang nhảy thì ko cho ấn S nữa
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
	if (simon->GetFreeze() == true) // Đang bóng băng thì không quan tâm phím
	{
		return;
	}
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
	LoadMap(TAG::MAP2);
	simon->Init();
}

void MainScene::ResetResource()
{
	gridGame->ReloadGrid();

	listItem.clear();
	listEffect.clear();

	camera->SetAllowFollowSimon(true);

	isHandlingGoThroughTheDoor1 = false; // Ban đầu chưa cần xử lí qua cửa
	isWentThroughTheDoor1 = false;

	isHandlingGoThroughTheDoor2 = false; // ban đầu chưa cần xử lí qua cửa
	isWentThroughTheDoor2 = false;
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

	// Phần xử lý map 2
#pragma region Xử lý đi qua Gate 1

	if (isHandlingGoThroughTheDoor1) // Simon chạm cửa thì bắt đầu xử lí
	{
		if (isWentThroughTheDoor1 == false) // simon chưa hoàn thành việc qua cửa
		{
			if (camera->GetXCam() >= GATE1_POSITION_CAM_BEFORE_GO) // camera đã AutoGo xong đến vị trí 2825.0f
			{
				simon->SetAutoGoX(1, 1, abs(GATE1_POSITION_CAM_AFTER_GO + DISTANCE_AUTO_WALK_AFTER_GATE - simon->GetX()), SIMON_WALKING_SPEED); // bắt đầu cho simon di chuyển tự động đến vị trí tiếp theo
			}
		}
		else
		{
			if (camera->GetXCam() >= GATE1_POSITION_CAM_AFTER_GO)
			{
				camera->SetBoundary(GATE1_POSITION_CAM_AFTER_GO, camera->GetBoundaryRight());

				camera->SetBoundaryBackup(camera->GetBoundaryLeft(), camera->GetBoundaryRight());

				camera->SetAllowFollowSimon(true);
				isHandlingGoThroughTheDoor1 = false; // Xong việc xử lí qua cửa 1
				camera->StopAutoGoX(); // Dừng việc tự di chuyển
			}
		}
	}

#pragma endregion

#pragma region Process Gate 2

	if (isHandlingGoThroughTheDoor2) // simon chạm cửa thì bắt đầu xử lí
	{
		if (isWentThroughTheDoor2 == false) // simon chưa hoàn thành việc qua cửa
		{
			if (camera->GetXCam() >= GATE2_POSITION_CAM_BEFORE_GO)
			{
				simon->SetAutoGoX(1, 1, abs(GATE2_POSITION_CAM_AFTER_GO + DISTANCE_AUTO_WALK_AFTER_GATE - simon->GetX()), SIMON_WALKING_SPEED); // bắt đầu cho simon di chuyển tự động đến vị trí tiếp theo
			}
		}
		else
		{
			if (camera->GetXCam() >= GATE2_POSITION_CAM_AFTER_GO)
			{
				camera->SetBoundary(GATE2_POSITION_CAM_AFTER_GO, camera->GetBoundaryRight());

				camera->SetBoundaryBackup(camera->GetBoundaryLeft(), camera->GetBoundaryRight());

				camera->SetAllowFollowSimon(true);
				isHandlingGoThroughTheDoor2 = false; // xong việc xử lí qua cửa 2
				camera->StopAutoGoX(); // dừng việc tự di chuyển
			}
		}
	}

#pragma endregion



	gridGame->GetListObject(listObj, camera);

	simon->Update(dt, &listObj);

	if (camera->AllowFollowSimon())
		camera->SetPosition(simon->GetX() - SCREEN_WIDTH / 2 + 30, camera->GetYCam()); // cho camera chạy theo simon

	camera->Update(dt);


#pragma region Update các object của map 2
#pragma endregion

#pragma region Phần update các object

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);

	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Update(dt, &listObj);

	for (UINT i = 0; i < listEffect.size(); i++)
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Update(dt);

#pragma endregion

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

void MainScene::CheckCollision()
{
	CheckCollisionWeapon(listObj);
	CheckCollisionSimonItem();
	CheckCollisionSimonAndHiddenObject();
	CheckCollisionSimonWithGate();
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

						/* Vũ khí phụ của simon */
						case TAG::ITEMDAGGER:
						{
							simon->PickUpSubWeapon(TAG::DAGGER);
							listItem[i]->SetFinish(true);
							break;
						}

						case TAG::CANDLE:
						{
							gameObj->SubHealth(1);
							listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
							RunEffectHit = true; // Hiệu ứng hit
							break;
						}

#pragma region Phần va chạm với gạch

						case TAG::BRICK:
						{
							if (objWeapon.second->GetType() != TAG::MORNINGSTAR) // nếu ko là MORNINGSTAR thì bỏ qua
								break;

							GameObject* gameObject = listObj[i];
							if (gameObject->GetHealth() > 0)
							{
								switch (gameObject->GetId())
								{
								case 39: // id 39 : brick 4 ô-> chỉ hiện effect, chỗ rơi đùi gà
								{
									gameObject->SubHealth(1);
									HIT_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng hit
									BROKEN_BRICK_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng BrokenBrick
									break;
								}

								case 40: // id 40: brick 3 ô-> effect, chỗ rơi đùi gà
								{
									gameObject->SubHealth(1);
									listItem.push_back(DropItem(gameObject->GetId(), gameObject->GetType(), gameObject->GetX(), gameObject->GetY()));
									HIT_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng hit
									BROKEN_BRICK_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng BrokenBrick
									break;
								}

								case 72: // id 72: brick -> bonus, map 2 phần ngoài cùng
								{
									gameObject->SubHealth(1);
									listItem.push_back(DropItem(gameObject->GetId(), gameObject->GetType(), gameObject->GetX(), gameObject->GetY()));
									HIT_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng hit
									BROKEN_BRICK_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng BrokenBrick
									break;
								}

								case 51: // id 51: brick 2 -> effect, dưới hồ bên phải ngoài cùng
								{
									gameObject->SubHealth(1);
									HIT_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng hit
									BROKEN_BRICK_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng BrokenBrick
									break;
								}

								case 104: // id 104: double shot, trong phần Boss
								{
									gameObject->SubHealth(1);

									listItem.push_back(DropItem(gameObject->GetId(), gameObject->GetType(), gameObject->GetX(), gameObject->GetY()));

									HIT_ADD_EFFECT(listEffect, gameObject); // hiệu ứng hit
									BROKEN_BRICK_ADD_EFFECT(listEffect, gameObject); // hiệu ứng BrokenBrick
									break;
								}

								}
							}
							break;
						}

#pragma endregion

						}

						if (RunEffectHit)
						{
							listEffect.push_back(new Hit(listObj[i]->GetX() + 10, listObj[i]->GetY() + 14)); // Hiệu ứng hit
							listEffect.push_back(new Fire(gameObj->GetX() - 5, gameObj->GetY() + 8)); // Hiệu ứng lửa

							
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

				case TAG::SMALLHEART:
				{
					simon->SetHeartCollect(simon->GetHeartCollect() + 1);
					listItem[i]->SetFinish(true);
					break;
				}

				/* Xử lí ăn tiền */
				case TAG::MONEY_RED_BAG:
				{
					listItem[i]->SetFinish(true);
					simon->SetScore(simon->GetScore() + 100);
					listEffect.push_back(new MoneyEffect(listItem[i]->GetX(), listItem[i]->GetY(), TAG::EFFECT_MONEY_100));
					break;
				}

				case TAG::MONEY_PURPLE_BAG:
				{
					listItem[i]->SetFinish(true);
					simon->SetScore(simon->GetScore() + 400);
					listEffect.push_back(new MoneyEffect(listItem[i]->GetX(), listItem[i]->GetY(), TAG::EFFECT_MONEY_400));
					break;
				}

				case TAG::MONEY_WHITE_BAG:
				{
					listItem[i]->SetFinish(true);
					simon->SetScore(simon->GetScore() + 700);
					listEffect.push_back(new MoneyEffect(listItem[i]->GetX(), listItem[i]->GetY(), TAG::EFFECT_MONEY_700));
					break;
				}

				case TAG::BONUS:
				{
					listItem[i]->SetFinish(true);
					simon->SetScore(simon->GetScore() + 1000);
					listEffect.push_back(new MoneyEffect(listItem[i]->GetX(), listItem[i]->GetY(), TAG::EFFECT_MONEY_1000));
					break;
				}
				/* Xử lí ăn tiền */

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

					if (mapCurrent == TAG::MAP2)
					{
						switch (object->GetId())
						{
						case 67: // Đụng trúng box xác nhận simon đã qua GATE1
						{
							if (isHandlingGoThroughTheDoor1)
							{
								isWentThroughTheDoor1 = true;
								camera->SetAutoGoX(abs(GATE1_POSITION_CAM_AFTER_GO - camera->GetXCam()), SIMON_WALKING_SPEED);
								//simon->SetPositionBackup(simon->GetX(), 0); // backup lại vị trí sau khi qua màn 
							}

							
							StageCurrent = 2;// set hiển thị đang ở state2
							object->SubHealth(1);

							//DebugOut(L"Xac nhan qua xong cua!\n");
							break;
						}

						case 65: //id 65 : object ẩn->bonus
						{
							listItem.push_back(DropItem(object->GetId(), object->GetType(), simon->GetX(), simon->GetY()));
							object->SetHealth(0);
							break;
						}

						case 66: //id 66: object ẩn -> chạm nước -> chết
						{
							simon->SetHealth(0);

							break;
						}

						case 94: // đụng trúng box xác nhận simon đã qua GATE2
						{
							if (isHandlingGoThroughTheDoor2)
							{
								// Di chuyển camera qua phần cuối
								isWentThroughTheDoor2 = true;
								camera->SetAutoGoX(abs(GATE2_POSITION_CAM_AFTER_GO - camera->GetXCam()), SIMON_WALKING_SPEED);
							}

							StageCurrent = 3;// set hiển thị đang ở state3
							object->SubHealth(1);

							DebugOut(L"Xac nhan qua xong cua 2!\n");
							break;
						}

#pragma region Lên & xuống hồ nước phía trái

						case 41: // id 41: object ẩn -> bắt đầu xuống hồ nước
						{
							camera->SetPosition(camera->GetXCam(), CAMERA_POSITION_Y_LAKE);
							camera->SetBoundary(CAMERA_BOUNDARY_LAKE_LEFT, CAMERA_BOUNDARY_LAKE_RIGHT);
							simon->SetPosition(3150, 405);
							object->SetHealth(0);


							gridGame->Insert(GRID_INSERT_OBJECT__GETOUTLAKE_LEFT); // thêm object ẩn để có thể đi lên

							break;
						}
						case 45: // id 45: object ẩn -> trở lên trước khi xuống hồ nước
						{
							camera->SetPosition(camera->GetXCam(), 0);
							simon->SetPosition(3152, 345);
							object->SetHealth(0);

							gridGame->Insert(GRID_INSERT_OBJECT__GETDOWNLAKE_LEFT); // thêm object ẩn để có thể đi xuống sau khi đã lên lại

							break;
						}
#pragma endregion



#pragma region Lên & xuống hồ nước phía Phải

						case 81: // id 81: object ẩn -> ra khỏi hồ nước phía phải
						{
							camera->SetPosition(camera->GetXCam(), 0);
							simon->SetPosition(3806, 361);
							object->SetHealth(0);
							// Thêm bên trái trong trường hợp Simon rớt ngược lại qua phần cửa đầu tiên
							gridGame->Insert(GRID_INSERT_OBJECT__GETDOWLAKE_RIGHT); // Thêm object ẩn để có thể đi xuống sau khi đã lên lại
							gridGame->Insert(GRID_INSERT_OBJECT__GETDOWNLAKE_LEFT); // Thêm object ẩn để có thể đi xuống sau khi đã lên lại
							break;
						}


						case 86: // id 86: object ẩn -> vào hồ nước phía phải
						{
							camera->SetPosition(camera->GetXCam(), CAMERA_POSITION_Y_LAKE);
							simon->SetPosition(3825, 450);

							object->SetHealth(0);

							gridGame->Insert(GRID_INSERT_OBJECT__GETOUTLAKE_RIGHT); // thêm object ẩn để có thể đi xuống sau khi đã lên lại

							break;
						}
#pragma endregion

						case 124: // id 124 : kích hoạt boss
						{
							//boss->Start();

							camera->SetBoundary(camera->GetBoundaryRight(), camera->GetBoundaryRight());
							camera->SetAllowFollowSimon(false);

							/*if (sound->isPlaying(eSound::musicState1))
							{
								sound->Stop(eSound::musicState1);
							}
							sound->Play(eSound::music_PhantomBat, true);*/

							object->SetHealth(0);
							break;
						}

						default:
							break;
						}
					}
				}
			}
		}
	}
}

void MainScene::CheckCollisionSimonWithGate()
{
	for (UINT i = 0; i < listObj.size(); i++)
	{
		if (listObj[i]->GetType() == TAG::GATE)
		{
			if (simon->isCollitionObjectWithObject(listObj[i]))
			{
				Gate* objGate = dynamic_cast<Gate*>(listObj[i]);


				if (mapCurrent == TAG::MAP2)
				{
					switch (objGate->GetId())
					{
					case 64: // gate 1
					{
						if (objGate->GetStart() == 0)
						{
							// Di chuyển camera đến GATE1_POSITION_CAM_BEFORE_GO = 2825.0f
							camera->SetBoundary(camera->GetBoundaryLeft(), camera->GetBoundaryRight() + 9999.0f);// Mở biên phải rộng ra thêm để chạy AutoGo
							camera->SetAutoGoX(abs(GATE1_POSITION_CAM_BEFORE_GO - camera->GetXCam()), SIMON_WALKING_SPEED);

#pragma region Stop simon

							simon->SetSpeed(0, simon->GetVy()); // Cho simon dừng, tránh trường hợp không vào được trạng thái stop trong KeyState()
							simon->isWalking = 0;
							if (simon->isSitting == true) // Nếu simon đang ngồi
							{
								simon->isSitting = 0; // Hủy trạng thái ngồi
								simon->SetY(simon->GetY() - PULL_UP_SIMON_AFTER_SITTING); // Kéo simon lên
							}
#pragma endregion

							isHandlingGoThroughTheDoor1 = true; // Bật trạng thái xử lí qua cửa
							isWentThroughTheDoor1 = false;
							objGate->Start();

							DebugOut(L"Simon dung trung cua!\n");


							break;
						}
						break;
					}

					case 93: // gate 2
					{
						if (objGate->GetStart() == 0)
						{
							camera->SetBoundary(camera->GetBoundaryLeft(), CAMERA_BOUNDARY_BOSS_RIGHT);// mở biên phải rộng ra thêm để chạy AutoGo
							camera->SetAutoGoX(abs(GATE2_POSITION_CAM_BEFORE_GO - camera->GetXCam()), SIMON_WALKING_SPEED);

#pragma region Stop simon
							simon->SetSpeed(0, simon->GetVy()); // cho simon dừng, tránh trường hợp không vào được trạng thái stop trong KeyState()
							simon->isWalking = 0;
							if (simon->isSitting == true) // nếu simon đang ngồi
							{
								simon->isSitting = 0; // hủy trạng thái ngồi
								simon->SetY(simon->GetY() - PULL_UP_SIMON_AFTER_SITTING); // kéo simon lên
							}
#pragma endregion

							isHandlingGoThroughTheDoor2 = true; // bật trạng thái xử lí qua cửa
							isWentThroughTheDoor2 = false;

							objGate->Start();
							DebugOut(L"Simon dung trung cua 2!\n");

							break;
						}
						break;
					}

					default:
						break;
					}

				}
			}
		}
	}
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

	if (mapCurrent == TAG::MAP2)
	{
		if (Type == TAG::CANDLE)
		{
			switch (Id)
			{
			case 2:
				return new MoneyBag(X, Y, TAG::MONEY_WHITE_BAG);
				break;

			default:
			{
				int random = rand() % 15;
				switch (random)
				{
				case 0:
					return new MoneyBag(X, Y, TAG::MONEY_RED_BAG);
					break;
				case 1:
					return new MoneyBag(X, Y, TAG::MONEY_WHITE_BAG);
					break;
				case 2:
					return new MoneyBag(X, Y, TAG::MONEY_PURPLE_BAG);
					break;

				default:
					return new SmallHeart(X, Y);
					break;
				}
				break;
			}
			}
		}

		if (Type == TAG::BRICK)
		{
			switch (Id)
			{
			case 40:
				//return new PotRoast(X, Y);
				break;

			case 72:
				return new Bonus(X, Y);
				break;

			case 104: // Double shot
				//return new ItemDoubleShot(X, Y);
				break;


			default:
				return new SmallHeart(X, Y);
				break;
			}
		}

		if (Type == TAG::OBJECT_HIDDEN)
		{
			if (Id == 65)
				return new Bonus(3763.0f, 587.0f);

		}
	}

	return new LargeHeart(X, Y);
}

#pragma endregion

