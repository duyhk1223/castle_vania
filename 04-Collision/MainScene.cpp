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

	if (simon->isHurting) // Simon đang bị thương thì ko quan tâm phím
		return;


	if (Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && simon->isProcessingOnStair == 0 && !simon->isAttacking)
	{
		simon->Attack(simon->GetTypeSubWeapon()); // Attack với vũ khí phụ Simon nhặt dc
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


	// Trạng thái nhảy theo 1 hướng thì ko thể attack hay đổi chiều Simon
	if (simon->isJumping && simon->isWalking)
		return;

	if (simon->isOnStair) // Nếu đang trên thang thì không xét loại đi trái phải
		return;
	
	// Simon tấn công khi đang nhảy
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
		return;

	if (simon->GetIsAutoGoX()) // Simon đang ơ chế độ tự đi thì ko xét phím
		return;

	if (camera->GetIsAutoGoX()) // Camera đang ở chế độ tự đi thì ko xét phím
		return;

	if (simon->isHurting) // Simon đang bị thương thì ko quan tâm phím
		return;

	if (!(Game::GetInstance()->IsKeyDown(DIK_UP) && Game::GetInstance()->IsKeyDown(DIK_A) && simon->isProcessingOnStair != 0 && simon->isAttacking == true))
		if (KeyCode == DIK_A && simon->isProcessingOnStair == 0) // Không phải đang xử lí việc đi trên thang thì đc đánh
		{
			simon->Attack(TAG::MORNINGSTAR);
		}


	// Nếu Simon đang nhảy thì ko bắt lệnh nhảy
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

#pragma region Keydown Debug

	if (KeyCode == DIK_U)
	{
		if (isDebug_Untouchable == 1)
			isDebug_Untouchable = 0;
		else
			isDebug_Untouchable = 1;
	}

#pragma endregion
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
	listEnemy.clear();
	listWeaponOfEnemy.clear();

	camera->SetAllowFollowSimon(true);

	isHandlingGoThroughTheDoor1 = false; // Ban đầu chưa cần xử lí qua cửa 1
	isWentThroughTheDoor1 = false;

	isHandlingGoThroughTheDoor2 = false; // Ban đầu chưa cần xử lí qua cửa 2
	isWentThroughTheDoor2 = false;

	isUsingInvisibilityPotion = false; // Ban đầu thì Simon chưa tàng hình
	isUsingCross = false; // Ban đầu Simon chưa nhắt dc thánh giá
	
	// Ghost
	CurrentGhostEnemyCount = 0;
	CreateGhostTime = 0;
	isWaitingToCreateGhost = false; // Lúc đầu thì tạo ghost luôn và không cần chờ

	// Black Panther
	isAllowCreatePanther = true;
	CurrentPantherEnemyCount = 0;

	// Bat
	CreateBatTime = 0;
	WaitingTimeToCreateBat = 0;
	isAllowToCreateBat = 0;

	// Fishmen
	isAllowToCreateFishmen = false;
	CreateFishmenTime = 0;
	WaitingtimeToCreateFishmen = 0;
	CurrentFishmenEnemyCount = 0;
}

void MainScene::Update(DWORD dt)
{
	// Xử lí freeze
	if (simon->GetFreeze() == true)
	{
		simon->UpdateFreeze(dt);

		if (simon->GetFreeze() == true)// dang freeze thì k update
			return;
	}

//===================================================================================================//

#pragma region Debug Update

	if (isDebug_Untouchable == 1)
		simon->StartUntouchable();

#pragma endregion

	// Phần xử lý map 2
#pragma region Xử lý đi qua Gate 1

	if (isHandlingGoThroughTheDoor1) // Simon chạm cửa thì bắt đầu xử lí
	{
		if (isWentThroughTheDoor1 == false) // Simon chưa hoàn thành việc qua cửa
		{
			if (camera->GetXCam() >= GATE1_POSITION_CAM_BEFORE_GO) // Camera đã AutoGo xong đến vị trí 2825.0f
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
//===================================================================================================//

#pragma region Xử lý qua Gate 2

	if (isHandlingGoThroughTheDoor2) // Siimon chạm cửa thì bắt đầu xử lí
	{
		if (isWentThroughTheDoor2 == false) // Simon chưa hoàn thành việc qua cửa
		{
			if (camera->GetXCam() >= GATE2_POSITION_CAM_BEFORE_GO) // Camera đã auto go xong đến vị trí 3840.0f
			{
				simon->SetAutoGoX(1, 1, abs(GATE2_POSITION_CAM_AFTER_GO + DISTANCE_AUTO_WALK_AFTER_GATE - simon->GetX()), SIMON_WALKING_SPEED); // Bắt đầu cho simon di chuyển tự động đến vị trí tiếp theo
			}
		}
		else
		{
			if (camera->GetXCam() >= GATE2_POSITION_CAM_AFTER_GO)
			{
				camera->SetBoundary(GATE2_POSITION_CAM_AFTER_GO, camera->GetBoundaryRight());

				camera->SetBoundaryBackup(camera->GetBoundaryLeft(), camera->GetBoundaryRight());

				camera->SetAllowFollowSimon(true);
				isHandlingGoThroughTheDoor2 = false; // Xong việc xử lí qua cửa 2
				camera->StopAutoGoX(); // Dừng việc tự di chuyển
			}
		}
	}

#pragma endregion

	//===================================================================================================//

	gridGame->GetListObject(listObj, camera);

	simon->Update(dt, &listObj);

	if (camera->AllowFollowSimon())
		camera->SetPosition(simon->GetX() - SCREEN_WIDTH / 2 + 30, camera->GetYCam()); // Cho camera chạy theo simon

	camera->Update(dt);

	//===================================================================================================//

					//============= Xử lý các vùng tạo enemy===========//

	if (mapCurrent == TAG::MAP2)
	{
#pragma region Vùng tạo Ghost

		if (mapCurrent == TAG::MAP2)
		{
			DWORD now = GetTickCount(); // Biến update giá trị thời gian để dùng cho việc reset thời gian tạo ghost

			if (isWaitingToCreateGhost == false) // Nếu không phải chờ xử lí thì vào xử lí tạo ghost
			{

#pragma region Vùng 1 & 2, phần trc khi qua cửa 1

				// Simon nằm trong vùng ngoài cùng của map 2 hoặc phần gần cầu thang qua cửa 1
				if ((simon->GetX() >= GHOST_ZONE1_LEFT && simon->GetX() <= GHOST_ZONE1_RIGHT) || (simon->GetX() > GHOST_ZONE2_LEFT && simon->GetX() < GHOST_ZONE2_RIGHT))
				{
					if (now - CreateGhostTime >= WAIT_TIME_BETWEEN_TWO_GHOST_IS_CREATED) // Nếu đã chờ >= 1s thì cho phép tạo Ghost
					{
						if (CurrentGhostEnemyCount < 3)
						{
							if (simon->GetVx() > 0) // vx > 0 thì Simon đang đi về bên phải
							{
								// Cho ghost chạy từ bên phải qua, hướng là -1
								listEnemy.push_back(new Ghost(camera->GetXCam() + camera->GetWidth(), 326 - 10, -1)); // 34 là framewidth của ghost
							}
							else
								if (simon->GetVx() < 0) // vx < 0 thì Simon đang đi về bên trái
								{
									// Cho ghost chạy từ bên trái qua , hướng là 1
									listEnemy.push_back(new Ghost(camera->GetXCam() - 34, 326 - 10, 1));
								}
								else // Nếu Simon đứng yên thì random
								{
									int random = rand() % 2;  // Tỉ lệ 50%
									if (random == 0) // Đi từ bên trái
									{
										listEnemy.push_back(new Ghost(camera->GetXCam() - 34, 326 - 10, 1));
									}
									else // Đi từ bên phải
									{
										listEnemy.push_back(new Ghost(camera->GetXCam() + camera->GetWidth(), 326 - 10, -1));
									}
								}

							CurrentGhostEnemyCount++;
							if (CurrentGhostEnemyCount == 3)
							{
								isWaitingToCreateGhost = true; // Phải chờ đến khi cả 3 ghost bị giết thì mới dc tạo tiếp
								isAllowCheckTimeWaitToCreateGhost = false;
							}
							CreateGhostTime = now; // Set lại thời điểm đã tạo ghost cuối
						}
					}
				}

#pragma endregion

#pragma region Vùng 3

				if ((simon->GetX() >= GHOST_ZONE3_LEFT && simon->GetX() <= GHOST_ZONE3_RIGHT)) // Simon ở giữa 2 vùng tạo Ghost
				{
					if (now - CreateGhostTime >= WAIT_TIME_BETWEEN_TWO_GHOST_IS_CREATED) // Nếu chờ >= 1s thì cho phép tạo Ghost
					{
						if (CurrentGhostEnemyCount < 3)
						{

							int random = rand() % 2; // Tỉ lệ 50%
							switch (random)
							{
							case 0: // Ghost ở trên, 2 vùng mỗi vùng tạo ra 1 con Ghost
							{
								if (simon->GetX() <= GHOST_ZONE3_COLUMN1)
								{
									listEnemy.push_back(new Ghost(camera->GetXCam() + camera->GetWidth(), 185, -1)); // Bên phải chạy qua trái
									break;
								}
								else
									if (GHOST_ZONE3_COLUMN2 <= simon->GetX())
									{
										listEnemy.push_back(new Ghost(camera->GetXCam() - 34, 185, 1)); // Bên trái qua phải
										break;
									}
							}
							case 1: // Ghost ở dưới, tạo ra 1 con
							{
								if (simon->GetVx() > 0) // Simon đang đi qua bên phải
									listEnemy.push_back(new Ghost(camera->GetXCam() + camera->GetWidth(), 330, -1));// Bên phải chạy qua trái
								else
									if (simon->GetVx() < 0) // Simon đang đi qua bên trái 
										listEnemy.push_back(new Ghost(camera->GetXCam() - 34, 330, 1)); // Đi từ trái qua phải
									else // Nếu Simon đứng yên thì random vị trí xuất hiện
									{
										if (rand() % 2 == 0)
											listEnemy.push_back(new Ghost(camera->GetXCam() + camera->GetWidth(), 330, -1)); // Bên phải chạy qua trái
										else
											listEnemy.push_back(new Ghost(camera->GetXCam() - 34, 330, 1)); // đi từ trái qua phải 
									}
								break;
							}

							}

							CurrentGhostEnemyCount++;
							if (CurrentGhostEnemyCount == 3)
							{
								isWaitingToCreateGhost = true; // Phải chờ đến khi cả 3 ghost bị giết hoặc out camera (chết)
								isAllowCheckTimeWaitToCreateGhost = false;
							}

							CreateGhostTime = now; // Set lại thời điểm đã tạo ghost cuối
						}
					}
				}
#pragma endregion

			}
			else
			{
				if (isAllowCheckTimeWaitToCreateGhost)
				{
					if (now - BeginWaitingToCreateGhostTime >= WAIT_TIME_BEFORE_ALLOW_TO_CREATE_GHOST) // Đã chờ đủ 2.5s
					{
						isWaitingToCreateGhost = false; // Không phải chờ nữa
					}
				}
			}
		}
#pragma endregion




#pragma region Vùng tạo Panther

		// Nếu Simon đang nằm trong vùng tạo báo thì cho phép tạo báo
		if (REGION_CREATE_PANTHER_BOUNDARY_LEFT < simon->GetX() && simon->GetX() < REGION_CREATE_PANTHER_BOUNDARY_RIGHT)
		{
			if (isAllowCreatePanther)
			{
				if (CurrentPantherEnemyCount == 0) // Không còn Panther nào sống thì mới dc tạo lại cả 3
				{
					// Hướng mặt của Panther quay về hướng simon
					int directionPanther = abs(REGION_CREATE_PANTHER_BOUNDARY_LEFT - simon->GetX()) < abs(REGION_CREATE_PANTHER_BOUNDARY_RIGHT - simon->GetX()) ? -1 : 1;

					listEnemy.push_back(new BlackPanther(1398.0f, 225.0f, directionPanther, directionPanther == -1 ? 20.0f : 9.0f, simon));
					listEnemy.push_back(new BlackPanther(1783.0f, 160.0f, directionPanther, directionPanther == -1 ? 278.0f : 180.0f, simon));
					listEnemy.push_back(new BlackPanther(1923.0f, 225.0f, directionPanther, directionPanther == -1 ? 68.0f : 66.0f, simon));
					CurrentPantherEnemyCount += 3;
				}
				isAllowCreatePanther = false;
			}
		}
		else // Nếu Simon ngoài vùng tạo báo thì dừng việc tạo báo
		{
			isAllowCreatePanther = true;
		}

#pragma endregion




#pragma region Vùng tạo dơi

		if (isAllowToCreateBat)
		{
			DWORD now = GetTickCount(); // Bắt đầu lấy thời gian để tính toán thời gian chờ tạo dơi
			if (now - CreateBatTime >= WaitingTimeToCreateBat) // Đủ thời gian chờ
			{
				CreateBatTime = now; // Đặt lại thời gian đã tạo bat
				// Xét toạ độ Simon đang đứng so với biên toạ độ để lấy hướng bay ra cho dơi

				// Ở bên phần 2 của screen trc cửa 2 thì dơi bay từ bên phải qua hoặc Simon mới qua cửa 1 (Chưa qua hồ cá), or Simon đứng trên phần gạch trc cửa 2
				if (simon->GetX() < CREATE_BAT_BOUNDARY_DIVISION_DIRECTION_X || (simon->GetX() > CREATE_BAT_BOUNDARY_DIVISION_DIRECTION_X && simon->GetY() > CREATE_BAT_BOUNDARY_DIVISION_DIRECTION_Y))
					listEnemy.push_back(new Bat(camera->GetXCam() + camera->GetWidth() - 10, simon->GetY() + 40, -1));
				else // Dơi bay từ bên trái qua khi Simon đã qua hồ cá
					listEnemy.push_back(new Bat(camera->GetXCam() - 10, simon->GetY() + 40, 1));

				WaitingTimeToCreateBat = 4000 + (rand() % 3000); // Random thời gian tạo dơi >= 4s
			}
		}

#pragma endregion




#pragma region Vùng tạo Fishmen

		if (isAllowToCreateFishmen && CurrentFishmenEnemyCount < 2) // Chỉ dc tạo 2 Fishmen
		{
			DWORD now = GetTickCount();
			if (now - CreateFishmenTime >= WaitingtimeToCreateFishmen) // Đã đủ thời gian chờ cho phép tạo Fishmen
			{
				CreateFishmenTime = now; // Đặt lại thời điểm đã tạo

				float appearPositionX = 0; // Vị trí xuất hiện random của Fishmen

#pragma region Xét vị trí Simon để random vị trí xuất hiện cho Fishmen

				if (FISHMEN_ZONE_1_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_1_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_3) : (FISHMEN_POS_4);
				}

				if (FISHMEN_ZONE_2_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_2_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_1) : ((rand() % 2) ? (FISHMEN_POS_3) : (FISHMEN_POS_4));
				}

				if (FISHMEN_ZONE_3_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_3_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_4) : (FISHMEN_POS_5);
				}

				if (FISHMEN_ZONE_4_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_4_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_3) : (FISHMEN_POS_5);
				}

				if (FISHMEN_ZONE_5_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_5_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_4) : (FISHMEN_POS_6);
				}

				if (FISHMEN_ZONE_6_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_6_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_5) : ((rand() % 2) ? (FISHMEN_POS_7) : (FISHMEN_POS_8));
				}

				if (FISHMEN_ZONE_7_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_7_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_6) : (FISHMEN_POS_8);
				}
				if (FISHMEN_ZONE_8_LEFT < simon->GetX() && simon->GetX() <= FISHMEN_ZONE_8_RIGHT)
				{
					appearPositionX = (rand() % 2) ? (FISHMEN_POS_6) : (FISHMEN_POS_7);
				}

#pragma endregion

				int directionFishmen = appearPositionX < simon->GetX() ? 1 : -1;


				float appearPositionY = FISHMEN_POS_Y;

				listEnemy.push_back(new Fishmen(appearPositionX, appearPositionY, directionFishmen, simon, &listWeaponOfEnemy, camera));
				CurrentFishmenEnemyCount++;

				STEAM_ADD_EFFECT(listEffect, appearPositionX, appearPositionY); // Thêm hiệu ứng bọt nước vào effect list

				//sound->Play(eSound::soundSplashwater);
				WaitingtimeToCreateFishmen = 2000 + (rand() % 2000); // >= 2s
			}
		}


#pragma endregion
	}


//===================================================================================================//

#pragma region Phần update các object

	for (UINT i = 0; i < listObj.size(); i++)
		listObj[i]->Update(dt, &listObj);  // Đã kiểm tra "Alive" lúc lấy từ lưới ra

	for (UINT i = 0; i < listItem.size(); i++)
		if (listItem[i]->GetFinish() == false)
			listItem[i]->Update(dt, &listObj); // Trong các hàm update chỉ kiểm tra va chạm với đất

	for (UINT i = 0; i < listEffect.size(); i++)
		if (listEffect[i]->GetFinish() == false)
			listEffect[i]->Update(dt);

	if (!simon->IsUsingWeapon(TAG::STOPWATCH)) // Nếu ko đang dùng StopWatch thì không update enemy
	{
		for (UINT i = 0; i < listEnemy.size(); i++)
		{
			GameObject* enemy = listEnemy[i];
			if (enemy->GetHealth() > 0) // Còn máu
			{
				switch (enemy->GetType())
				{
				case TAG::GHOST:
				{
					if (camera->CHECK_OBJECT_IN_CAMERA(enemy) == false)  // Vượt khỏi cam
					{
						enemy->SetHealth(0); // Ra khỏi cam thì coi như enemy đã chết
						CurrentGhostEnemyCount--; // Giảm số lượng ghost hiện tại
						if (CurrentGhostEnemyCount == 0)
						{
							BeginWaitingToCreateGhostTime = GetTickCount(); // Set thời điểm hiện tại để tính toán thời gian cho phép tạo lại Ghost
							isWaitingToCreateGhost = true;
							isAllowCheckTimeWaitToCreateGhost = true;
						}
					}
					else
						enemy->Update(dt, &listObj);
					break;
				}

				case TAG::PANTHER:
				{
					if (camera->CHECK_OBJECT_IN_CAMERA(enemy)) // nếu Panther nằm trong camera thì update
						// Vì do Grid load object nền (Brick) dựa vào vùng camera, nên có nguy cơ khiến 1 số object Panther không xét được va chạm đất
					{
						enemy->Update(dt, &listObj);
					}
					else // Nằm ngoài camera
					{
						BlackPanther* objPanther = dynamic_cast<BlackPanther*>(enemy);
						if (objPanther->GetIsStart())// ngoài cam và đã được kích hoạt r
						{
							objPanther->SetHealth(0); // cho Panther chết
							CurrentPantherEnemyCount--;
						}
					}
					break;
				}

				case TAG::BAT:
				{
					if (isAllowToCreateBat)
					{
						if (camera->CHECK_OBJECT_IN_CAMERA(enemy)) // Nếu nằm trong camera thì update
						{
							enemy->Update(dt);
						}
						else
						{
							enemy->SetHealth(0); // Ra khỏi cam coi như chết
						}
					}
					else enemy->SetHealth(0); // Trong trường hợp Simon đụng trúng cửa 2

					break;
				}

				case TAG::FISHMEN:
				{
					if (camera->CHECK_OBJECT_IN_CAMERA(enemy)) // Nếu nằm trong camera thì update
					{
						enemy->Update(dt, &listObj);
					}
					else
					{
						enemy->SetHealth(0); // Ra khỏi cam coi như chết
						CurrentFishmenEnemyCount--;
					}
					break;
				}

				
				default:
					break;
				}
			}
		}

		/*if (boss != NULL)
			boss->Update(dt, &listObj);*/
	}


	// Update vũ khí của enemy
	for (UINT i = 0; i < listWeaponOfEnemy.size(); i++)
	{
		if (listWeaponOfEnemy[i]->GetFinish() == false)
		{
			listWeaponOfEnemy[i]->Update(dt, &listObj);
		}
	}

#pragma endregion

//===================================================================================================//

#pragma region Các update khác

	CheckCollision();
	HandleInvisibilityPotion(dt);
	HandleCross(dt);

#pragma endregion
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

	for (UINT i = 0; i < listEnemy.size(); i++)
		listEnemy[i]->Render(camera);

	for (UINT i = 0; i < listWeaponOfEnemy.size(); i++)
		listWeaponOfEnemy[i]->Render(camera);

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

	if (!isHandlingGoThroughTheDoor1 && !isHandlingGoThroughTheDoor2) // Ko phải đang xử lí qua cửa
		CheckCollisionWithEnemy(); // Kiểm tra va chạm vũ khí với enemy và Simon với enemy
}

void MainScene::CheckCollisionWeapon(vector<GameObject*> listObj) // Kiểm tra va chạm của vũ khí 
{
	for (auto& objWeapon : simon->mapWeapon)
	{
		if (objWeapon.second->GetFinish() == false) // Vũ khí không đang hoạt động
		{


			for (UINT i = 0; i < listObj.size(); i++) {
				if (objWeapon.second->GetLastTimeAttack() > listObj[i]->GetLastTimeAttacked()) // Nếu chưa xét va chạm của lượt attack này ở các frame trước
				{
					if (objWeapon.second->isCollision(listObj[i]) == true) // Nếu có va chạm
					{
						bool RunEffectHit = false;
						GameObject* gameObj = listObj[i];
						switch (gameObj->GetType()) {

						case TAG::TORCH:
							gameObj->SubHealth(1);
							listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
							RunEffectHit = true;
							break;

						case TAG::CANDLE:
						{
							gameObj->SubHealth(1);
							listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
							RunEffectHit = true; // Hiệu ứng hit
							break;
						}


#pragma region Phần va chạm với Enemy

						case TAG::GHOST:
						{
							gameObj->SubHealth(1);
							simon->SetScore(simon->GetScore() + 100);
							if (rand() % 2 == 1) // tỉ lệ 50%
							{
								listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
							}

							RunEffectHit = true;
							CurrentGhostEnemyCount--; // giảm số lượng Ghost đang hoạt động
							if (CurrentGhostEnemyCount == 0)
							{
								BeginWaitingToCreateGhostTime = GetTickCount(); // set thời điểm hiện tại
								isWaitingToCreateGhost = true;
								isAllowCheckTimeWaitToCreateGhost = true;
							}
							break;
						}

						case TAG::PANTHER:
						{
							gameObj->SubHealth(1);
							simon->SetScore(simon->GetScore() + 200);
							if (rand() % 2 == 1) // tỉ lệ 50%
							{
								listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));
							}
							RunEffectHit = true;
							CurrentPantherEnemyCount--; // giảm số lượng Panther đang hoạt động
							break;
						}

						case TAG::BAT:
						{
							gameObj->SubHealth(1);
							simon->SetScore(simon->GetScore() + 200);
							if (rand() % 2 == 1) // Tỉ lệ 50%
							{
								listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

							}

							RunEffectHit = true;
							break;
						}

						case TAG::FISHMEN:
						{
							gameObj->SubHealth(1);
							simon->SetScore(simon->GetScore() + 300);
							if (rand() % 2 == 1) // tỉ lệ 50% 
								listItem.push_back(DropItem(gameObj->GetId(), gameObj->GetType(), gameObj->GetX() + 5, gameObj->GetY()));

							RunEffectHit = true;
							CurrentFishmenEnemyCount--; // giảm số lượng Fishmen đang hoạt động

							break;
						}

#pragma endregion



#pragma region Phần va chạm với gạch

						case TAG::BRICK:
						{
							if (objWeapon.second->GetType() != TAG::MORNINGSTAR) // Nếu ko là MORNINGSTAR thì bỏ qua
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

									HIT_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng hit
									BROKEN_BRICK_ADD_EFFECT(listEffect, gameObject); // Hiệu ứng BrokenBrick
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
		if (listItem[i]->GetFinish() == false && listItem[i]->IsWaitingToDisplay() == false) // Chưa kết thúc thời gian hiển thị và không phải đang chờ để hiển thị
		{
			if (simon->isCollisionWithItem(listItem[i]) == true) // Có va chạm với item
			{
				switch (listItem[i]->GetType())
				{

				// Các item khác
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

				
				case TAG::POTROAST:
				{
					listItem[i]->SetFinish(true);
					simon->SetHealth(min(simon->GetHealth() + 6, SIMON_DEFAULT_HEALTH)); // Tăng 6 đơn vị máu
					break;
				}

				case TAG::ITEMDOUBLESHOT:
				{
					simon->SetIsUsingDoubleShot(true); // Cho phép chế độ Double Shot
					listItem[i]->SetFinish(true);
					break;
				}

				case TAG::SMALLHEART:
				{
					simon->SetHeartCollect(simon->GetHeartCollect() + 1);
					listItem[i]->SetFinish(true);
					break;
				}

				//====================================================================//


				//Sub weapon item
				case TAG::ITEMDAGGER:
				{
					simon->PickUpSubWeapon(TAG::DAGGER);
					listItem[i]->SetFinish(true);
					break;
				}

				case TAG::ITEMHOLYWATER:
				{
					simon->PickUpSubWeapon(TAG::HOLYWATER);
					listItem[i]->SetFinish(true);

					break;
				}

				case TAG::ITEMTHROWINGAXE:
				{
					simon->PickUpSubWeapon(TAG::THROWINGAXE);

					listItem[i]->SetFinish(true);
					break;
				}

				case TAG::INVISIBILITYPOTION:
				{
					isUsingInvisibilityPotion = true;
					simon->SetTexture(TextureManager::GetInstance()->GetTexture(TAG::SIMON_TRANS));
					listItem[i]->SetFinish(true);
					break;
				}

				case TAG::ITEMBOOMERANG:
				{
					simon->PickUpSubWeapon(TAG::BOOMERANG);
					listItem[i]->SetFinish(true);
					break;
				}

				case TAG::STOPWATCH:
				{
					simon->PickUpSubWeapon(TAG::STOPWATCH);
					listItem[i]->SetFinish(true);
					break;
				}

				// Nếu Simon nhặt dc thánh giá thì sẽ xoá hết enemy trong map
				case TAG::CROSS:
				{
					isUsingCross = true;
					Cross_WaitedTime = 0;
					Cross_ChangeColorBackground_WaitedTime = 0;

					board->SetTexure(TextureManager::GetInstance()->GetTexture(TAG::BOARD_TRANS)); // đổi thành Board màu nền trong suốt

					/*Xóa hết enemy*/
					for (UINT k = 0; k < listEnemy.size(); k++)
					{
						GameObject* enemy = listEnemy[k];
						if (enemy->GetHealth() > 0) // còn máu
						{
							enemy->SetHealth(0);
							listEffect.push_back(new Fire(enemy->GetX() - 5, enemy->GetY() + 8)); // hiệu ứng lửa
						}
					}
					CurrentGhostEnemyCount = 0;
					BeginWaitingToCreateGhostTime = GetTickCount(); // Set thời điểm hiện tại
					isWaitingToCreateGhost = true;
					isAllowCheckTimeWaitToCreateGhost = true;

					//CountEnemyFishmen = 0;
					//CountEnemyBat = 0;

					CurrentPantherEnemyCount = 0;
					
					/*Xóa hết enemy*/

					listItem[i]->SetFinish(true);
					//sound->Play(eSound::soundHolyCross);
					break;
				}
				

				//=====================================================================//

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

							
							StageCurrent = 2;// Set hiển thị đang ở stage 2
							object->SubHealth(1);

							// Cho phép tạo dơi
							CreateBatTime = GetTickCount();
							WaitingTimeToCreateBat = 3000;
							isAllowToCreateBat = true;

							DebugOut(L"Xac nhan qua xong cua 1!\n");
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

						case 94: // Đụng trúng box xác nhận simon đã qua GATE2
						{
							if (isHandlingGoThroughTheDoor2)
							{
								// Di chuyển camera qua phần cuối
								isWentThroughTheDoor2 = true;
								camera->SetAutoGoX(abs(GATE2_POSITION_CAM_AFTER_GO - camera->GetXCam()), SIMON_WALKING_SPEED);
							}

							StageCurrent = 3;// set hiển thị đang ở state3
							object->SubHealth(1);

							//isAllowToCreateBat = false; // Ngưng không tạo Bat nữa

							DebugOut(L"Xac nhan qua xong cua 2!\n");
							break;
						}

#pragma region Lên & xuống hồ nước phía trái

						case 41: // id 41: object ẩn -> Bắt đầu xuống hồ nước
						{
							camera->SetPosition(camera->GetXCam(), CAMERA_POSITION_Y_LAKE);
							camera->SetBoundary(CAMERA_BOUNDARY_LAKE_LEFT, CAMERA_BOUNDARY_LAKE_RIGHT);
							simon->SetPosition(3150, 405);
							object->SetHealth(0);

							isAllowToCreateBat = false;  // Không cho tạo Bat
							isAllowToCreateFishmen = true; // Cho phép tạo Fishmen

							gridGame->Insert(GRID_INSERT_OBJECT__GETOUTLAKE_LEFT); // Thêm object ẩn để có thể đi lên

							break;
						}
						case 45: // id 45: object ẩn -> Trở lên trước khi xuống hồ nước
						{
							camera->SetPosition(camera->GetXCam(), 0);
							simon->SetPosition(3152, 345);
							object->SetHealth(0);

							isAllowToCreateBat = true;  // Cho phép tạo Bat
							isAllowToCreateFishmen = false; // Ngừng việc tạo Fishmen

							gridGame->Insert(GRID_INSERT_OBJECT__GETDOWNLAKE_LEFT); // Thêm object ẩn để có thể đi xuống sau khi đã lên lại

							break;
						}
#pragma endregion



#pragma region Lên & xuống hồ nước phía Phải

						case 81: // id 81: object ẩn -> Ra khỏi hồ nước phía phải
						{
							camera->SetPosition(camera->GetXCam(), 0);
							simon->SetPosition(3806, 361);
							object->SetHealth(0);

							isAllowToCreateBat = true;
							WaitingTimeToCreateBat = 3000 + rand() % 1000; // Random thời gian chờ tạo Bat, >= 4s

							isAllowToCreateFishmen = false; // Ngừng việc tạo Fishmen

							// Thêm bên trái trong trường hợp Simon rớt ngược lại qua phần cửa đầu tiên
							gridGame->Insert(GRID_INSERT_OBJECT__GETDOWLAKE_RIGHT); // Thêm object ẩn để có thể đi xuống sau khi đã lên lại
							gridGame->Insert(GRID_INSERT_OBJECT__GETDOWNLAKE_LEFT); // Thêm object ẩn để có thể đi xuống sau khi đã lên lại
							break;
						}


						case 86: // id 86: object ẩn -> Vào hồ nước phía phải
						{
							camera->SetPosition(camera->GetXCam(), CAMERA_POSITION_Y_LAKE);
							simon->SetPosition(3825, 450);
							object->SetHealth(0);

							isAllowToCreateBat = false;  // Không cho tạo Bat
							isAllowToCreateFishmen = true; // Cho phép tạo Fishmen

							gridGame->Insert(GRID_INSERT_OBJECT__GETOUTLAKE_RIGHT); // Thêm object ẩn để có thể đi xuống sau khi đã lên lại

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
					case 64: // Gate 1
					{
						if (objGate->GetStart() == 0)
						{
							// Di chuyển camera đến GATE1_POSITION_CAM_BEFORE_GO = 2825.0f
							camera->SetBoundary(camera->GetBoundaryLeft(), camera->GetBoundaryRight() + 9999.0f); // Mở biên phải rộng ra thêm để chạy AutoGo
							camera->SetAutoGoX(abs(GATE1_POSITION_CAM_BEFORE_GO - camera->GetXCam()), SIMON_WALKING_SPEED);

#pragma region Stop simon

							simon->SetSpeed(0, simon->GetVy()); // Cho Simon dừng, tránh trường hợp không vào được trạng thái stop trong KeyState()
							simon->isWalking = 0;
							if (simon->isSitting == true) // Nếu simon đang ngồi
							{
								simon->isSitting = 0; // Hủy trạng thái ngồi
								simon->SetY(simon->GetY() - PULL_UP_SIMON_AFTER_SITTING); // Kéo Simon lên, trừ đi để bằng với kích thước frame, do khi ngồi phải trừ đi phần khoảng trống dưới frame ngồi
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

					case 93: // Gate 2
					{
						if (objGate->GetStart() == 0)
						{
							camera->SetBoundary(camera->GetBoundaryLeft(), CAMERA_BOUNDARY_BOSS_RIGHT); // Mở biên phải rộng ra thêm để chạy AutoGo
							camera->SetAutoGoX(abs(GATE2_POSITION_CAM_BEFORE_GO - camera->GetXCam()), SIMON_WALKING_SPEED);

#pragma region Stop simon
							simon->SetSpeed(0, simon->GetVy()); // Cho simon dừng, tránh trường hợp không vào được trạng thái stop trong KeyState()
							simon->isWalking = 0;
							if (simon->isSitting == true) // Nếu simon đang ngồi
							{
								simon->isSitting = 0; // Hủy trạng thái ngồi
								simon->SetY(simon->GetY() - PULL_UP_SIMON_AFTER_SITTING); // Kéo simon lên
							}
#pragma endregion

							isHandlingGoThroughTheDoor2 = true; // Bật trạng thái xử lí qua cửa
							isWentThroughTheDoor2 = false;

							isAllowToCreateBat = false; // Ngừng việc tạo dơi

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

void MainScene::CheckCollisionWithEnemy()
{
	CheckCollisionWeapon(listEnemy); // Xét va chạm vũ khí với enemy
	CheckCollisionSimonWithEnemy(); // Xét va chạm giữa Simon và enemy
}

void MainScene::CheckCollisionSimonWithEnemy()
{
	// Nếu Simon đã ở trạng thái ko thể va chạm đủ thời gian tối đa thì sẽ có thể va chạm lại
	if (GetTickCount() - simon->startUntouchableTime > SIMON_UNTOUCHABLE_TIME)
	{
		simon->startUntouchableTime = 0;
		simon->isUntouchable = false;
	}

	if (isUsingInvisibilityPotion) // Ko sử dụng thuốc tàng hình thì mới xét va chạm
	{
		return;
	}

	// Phần xét va chạm với enemy
	if (simon->isUntouchable == false) // Nếu Simon ko còn trong trạng thái ko thể va chạm thì sẽ xét va chạm bình thường
	{
#pragma region Va chạm với Enemy bình thường
		for (UINT i = 0; i < listEnemy.size(); i++)
		{
			GameObject* gameobj = dynamic_cast<GameObject*> (listEnemy[i]);
			if (gameobj->GetHealth() > 0) // Enemy còn sống
			{
				LPCOLLISIONEVENT e = simon->SweptAABBEx(gameobj);
				bool isCollision = false;
				if (e->t > 0 && e->t <= 1) // Có va chạm
				{
					simon->SetHurt(e);
					isCollision = true;
				}
				if (isCollision == false && simon->checkAABB(gameobj) == true)
				{
					LPCOLLISIONEVENT e = new CollisionEvent(1.0f, (float)-simon->GetDirection(), 0.0f, NULL);
					simon->SetHurt(e);
					isCollision = true;
				}

				if (isCollision)
				{
					if (gameobj->GetType() == TAG::BAT) // Nếu va chạm với dơi thì dơi sẽ chết
					{
						listEffect.push_back(new Fire(gameobj->GetX() - 5, gameobj->GetY() + 8)); // Hiệu ứng lửa
						gameobj->SetHealth(0);
					}
					return; // Giảm chi phí duyệt, vì nếu Simon đang ở trạng thái untouchable thì ko xét va chạm
				}
			}
		}
#pragma endregion 
	}


	// Phần xét va chạm với weapon của enemy
	if (simon->isUntouchable == false)
	{
#pragma region Phần xét va chạm với vũ khí của enemy

		for (UINT i = 0; i < listWeaponOfEnemy.size(); i++)
		{
			if (listWeaponOfEnemy[i]->GetFinish() == false)
			{
				LPCOLLISIONEVENT e = simon->SweptAABBEx(listWeaponOfEnemy[i]);
				if (e->t > 0 && e->t <= 1) // Có va chạm
				{
					simon->SetHurt(e);
					return; // Giảm chi phí duyệt, vì nếu Simon đang ở trạng thái untouchable thì ko xét va chạm
				}

				if (simon->checkAABB(listWeaponOfEnemy[i]) == true) // Nếu có va chạm
				{
					LPCOLLISIONEVENT e = new CollisionEvent(1.0f, (float)-simon->GetDirection(), 0.0f, NULL);
					simon->SetHurt(e);
					return;
				}
			}
		}
#pragma endregion
	}
}

Item* MainScene::DropItem(int Id, TAG Type, float X, float Y) // Xử lí rớt item
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
#pragma region Phần rơi đồ khi đánh nến

		if (Type == TAG::CANDLE)
		{
			switch (Id)
			{
			case 2: // Ngay đầu map 2
				return new MoneyBag(X, Y, TAG::MONEY_WHITE_BAG);
				break;

				// Temp
			/*case 3:
				return new ItemThrowingAxe(X, Y);
				break;
			case 4:
				return new ItemDoubleShot(X, Y);
				break;*/

			case 23: case 98: // Trc 2 cửa
				return new Cross(X, Y);
				break;

			case 71: // Nằm ở phần có báo đen
				return new ItemHolyWater(X, Y);
				break;

			case 76:
				return new ItemStopWatch(X, Y);
				break;

			case 109: // Nằm gần gate 2
				return new InvisibilityPotion(X, Y);
				break;

			case 111: // Nằm gần cầu thang gate 2
				return new ItemThrowingAxe(X, Y);
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

#pragma endregion


#pragma region Phần rơi đồ khi đánh Enemy

		if (Type == TAG::GHOST || Type == TAG::PANTHER || Type == TAG::BAT || Type == TAG::FISHMEN)
		{
			int random = rand() % 15;

			if (random == 4) // Nếu random ra upgrade MorningStar
			{
				MorningStar* morningstar = dynamic_cast<MorningStar*>(simon->mapWeapon[TAG::MORNINGSTAR]);
				while (morningstar->GetLevel() == 2 && random == 4) // Nếu MorningStar đã max level thì random lại
				{
					random = rand() % 15;
				}
			}


			switch (random)
			{
			case 0:
				return new LargeHeart(X, Y);
				break;
			case 1:
				return new SmallHeart(X, Y);
				break;
			case 2:
				return new ItemDagger(X, Y);
				break;
			case 3:
				return new Bonus(X, Y);
				break;
			case 4:
				return new UpgradeMorningStar(X, Y);
				break;
			case 5:
				return new ItemHolyWater(X, Y);
				break;
			case 6:
				return new ItemStopWatch(X, Y);
				break;
			case 7:
				return new ItemThrowingAxe(X, Y);
				break;
			case 8:
				return new InvisibilityPotion(X, Y);
				break;
			case 9:
				return new ItemBoomerang(X, Y);
				break;
			default: // còn lại là SmallHeart
				return new SmallHeart(X, Y);
				break;
			}
		}

#pragma endregion


#pragma region Phần rơi đồ khi đánh gạch

		if (Type == TAG::BRICK)
		{
			switch (Id)
			{
			case 40: // Đùi gà
				return new PotRoast(X, Y);
				break;

			case 72: // Phần ngoài cùng của map 2
				return new Bonus(X, Y);
				break;

			case 104: // Double shot, trong phần đánh boss
				return new ItemDoubleShot(X, Y);
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

#pragma endregion

	return new LargeHeart(X, Y);
}

#pragma endregion


#pragma region Các hàm xử lý khác

void MainScene::HandleInvisibilityPotion(DWORD dt)
{
	if (isUsingInvisibilityPotion)
	{
		InvisibilityPotion_WaitedTime += dt; // Đếm thời gian sử dụng thuốc
		// Nếu đã hết thời gian sử dụng thì sẽ ngưng
		if (InvisibilityPotion_WaitedTime >= INVISIBILITYPOTION_LIMITTIMEWAIT)
		{
			isUsingInvisibilityPotion = false; // Kết thúc
			InvisibilityPotion_WaitedTime = 0;
			//sound->Play(eSound::soundInvisibilityPotion_End);

			simon->SetTexture(TextureManager::GetInstance()->GetTexture(TAG::SIMON));
		}
	}
}

void MainScene::HandleCross(DWORD dt)
{
	if (isUsingCross)
	{
		/* Xử lí thời gian hoạt động, nếu hết thời gian sử dụng thì trả về bình thường*/
		Cross_WaitedTime += dt;
		if (Cross_WaitedTime >= CROSS_MAXUSINGTIME)
		{
			isUsingCross = false;
			D3DBACKGROUND_COLOR = BACKGROUND_DEFAULT_COLOR; // Trả về màu nền mặc định
			board->SetTexure(TextureManager::GetInstance()->GetTexture(TAG::BOARD)); // Đổi thành Board màu bt
		}
		else
		{
			/*Xử lí đổi màu nền*/
			Cross_ChangeColorBackground_WaitedTime += dt;
			if (Cross_ChangeColorBackground_WaitedTime >= Cross_ChangeColorBackground_MaxWaitingTime) // Xét xem đã tới thời điểm đổi màu nên hay chưa
			{
				Cross_ChangeColorBackground_WaitedTime = 0;
				Cross_ChangeColorBackground_MaxWaitingTime = rand() % 100; // Giảm dần thời gian đổi màu
				/*Đổi màu nền*/
				if (D3DBACKGROUND_COLOR == BACKGROUND_DEFAULT_COLOR)
				{
					D3DBACKGROUND_COLOR = CROSS_COLOR_BACKGROUND;
				}
				else
				{
					D3DBACKGROUND_COLOR = BACKGROUND_DEFAULT_COLOR;
				}
			}
		}
	}
}

#pragma endregion

