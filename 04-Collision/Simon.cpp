#include "Simon.h"


Simon::Simon(Camera* camera)
{
	texture = TextureManager::GetInstance()->GetTexture(TAG::SIMON);
	sprite = new Sprite(texture, 150);
	// _sprite_deadth = new Sprite(TextureManager::GetInstance()->GetTexture(TAG::SIMON_DEADTH), 250);
	type = TAG::SIMON;


	this->camera = camera;
	// this->sound = Sound::GetInstance();
	mapWeapon[TAG::MORNINGSTAR] = new MorningStar();

	Init();
}

Simon::~Simon()
{
}

void Simon::Init()
{
	Health = SIMON_DEFAULT_HEALTH; // Simon dính 16 phát là chết
	Lives = SIMON_DEFAULT_LIVES; // Simon có 3 mạng
	HeartCollect = SIMON_DEFAULT_HEARTCOLLECT; // Simon có 5 tim ban đầu
	Score = SIMON_DEFAULT_SCORE; // Ban đầu chưa có điểm

	Reset();
}

void Simon::Reset()
{
	direction = 1;

	isSitting = 0;
	isJumping = 0;
	isWalking = 0;
	isAttacking = 0;
	isOnStair = 0;
	isProcessingOnStair = 0;// Ko phải đang xử lí lên thang

	isAutoGoX = 0;

	passedDistance = 0;
	isFreeze = 0;
	TimeFreeze = 0;

	vx = 0;
	vy = 0;

	isUsingDoubleShot = false;
	SubWeaponType = TAG::NO_SUBWEAPON;
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isSitting == true)
	{
		left = x + 15;
		top = y - 1;
		right = x + SIMON_BBOX_WIDTH - 15;
		bottom = y + SIMON_BBOX_SITTING_HEIGHT;
	}
	else
	{
		left = x + 15;
		top = y - 1;
		right = x + SIMON_BBOX_WIDTH - 15;
		bottom = y + SIMON_BBOX_HEIGHT;

		if (isJumping)
			bottom = y + SIMON_BBOX_JUMPING_HEIGHT;
	}
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x < camera->GetBoundaryLeft() - 16)
		x = camera->GetBoundaryLeft() - 16;
	if (x + SIMON_BBOX_WIDTH > camera->GetBoundaryRight() + SCREEN_WIDTH)
		x = (float)(camera->GetBoundaryRight() + SCREEN_WIDTH - SIMON_BBOX_WIDTH);
	/* Không cho Simon lọt khỏi camera */


#pragma region Update sprite

	int index = sprite->GetCurrentFrame();

#pragma region Xử lý khi Simon ở trên cầu thang

	if (isOnStair)
	{
		if (isAttacking == true) // tấn công
		{
			if (directionY == -1) // đang đi lên
			{
				/* Xử lí ani đánh khi đang đi lên thang*/
				if (index < SIMON_ANI_STAIR_UP_ATTACKING_BEGIN) // nếu ani chưa đúng
				{
					sprite->SelectFrame(SIMON_ANI_STAIR_UP_ATTACKING_BEGIN); // set lại ani bắt đầu
					sprite->timeAccumulated = dt;
				}
				else
				{
					/* Update ani bình thường */
					sprite->timeAccumulated += dt;
					if (sprite->timeAccumulated >= SIMON_TIME_WAIT_ANI_ATTACKING)
					{
						sprite->timeAccumulated -= SIMON_TIME_WAIT_ANI_ATTACKING;
						sprite->SelectFrame(sprite->GetCurrentFrame() + 1);
					}
					/* Update ani bình thường */

					if (sprite->GetCurrentFrame() > SIMON_ANI_STAIR_UP_ATTACKING_END) // đã đi vượt qua frame cuối
					{
						isAttacking = false;
						sprite->SelectFrame(SIMON_ANI_STAIR_STANDING_UP);
					}
				}
				/* Xử lí ani đánh khi đang đi lên thang*/

			}
			else
			{
				/* Xử lí ani đánh khi đang đi xuống thang*/
				if (index < SIMON_ANI_STAIR_DOWN_ATTACKING_BEGIN) // nếu ani chưa đúng
				{
					sprite->SelectFrame(SIMON_ANI_STAIR_DOWN_ATTACKING_BEGIN); // set lại ani bắt đầu
					sprite->timeAccumulated = dt;
				}
				else
				{
					/* Update ani bình thường */

					// Bắt đầu tấn công
					sprite->timeAccumulated += dt;
					if (sprite->timeAccumulated >= SIMON_TIME_WAIT_ANI_ATTACKING)
					{
						sprite->timeAccumulated -= SIMON_TIME_WAIT_ANI_ATTACKING;
						sprite->SelectFrame(sprite->GetCurrentFrame() + 1);
					}

					/* Update ani bình thường */

					// Đã hoàn thành ani tấn công
					if (sprite->GetCurrentFrame() > SIMON_ANI_STAIR_DOWN_ATTACKING_END) // đã đi vượt qua frame cuối
					{
						isAttacking = false;
						sprite->SelectFrame(SIMON_ANI_STAIR_STANDING_DOWN);
					}
				}
				/* Xử lí ani đánh khi đang đi xuống thang*/

			}



		}
		else
		{
			if (isWalking == true)
			{
				if (isProcessingOnStair == 1) // Nếu ở giai đoạn bước chân thì set frame 12
				{
					if (vy < 0) // ddi len
						sprite->SelectFrame(SIMON_ANI_STAIR_GO_UP_BEGIN);
					else
						sprite->SelectFrame(SIMON_ANI_STAIR_GO_DOWN_BEGIN);
				}


				if (isProcessingOnStair == 2) // Nếu ở giai đoạn bước chân trụ thì set frame 13
				{
					if (vy < 0) // ddi len
						sprite->SelectFrame(SIMON_ANI_STAIR_GO_UP_END);
					else
						sprite->SelectFrame(SIMON_ANI_STAIR_GO_DOWN_END);
				}

				passedDistance = passedDistance + abs(vy) * 16.0f;

				if (passedDistance >= 8.0f && isProcessingOnStair == 1)
					isProcessingOnStair++;

				if (passedDistance >= 16)
				{
					isProcessingOnStair++;

					/* Fix lỗi mỗi lần đi vượt quá 16px */
					if (direction == 1 && directionY == -1) // Đi lên bên phải
					{
						x -= (passedDistance - 16.0f);
						y += (passedDistance - 16.0f);
					}
					if (direction == -1 && directionY == -1) // Đi lên bên trái
					{
						x += (passedDistance - 16.0f);
						y += (passedDistance - 16.0f);
					}

					if (direction == 1 && directionY == 1) // Đi xuống bên phải
					{
						x -= (passedDistance - 16.0f);
						y -= (passedDistance - 16.0f);
					}
					if (direction == -1 && directionY == 1) // Đi xuống bên trái
					{
						x += (passedDistance - 16.0f);
						y -= (passedDistance - 16.0f);
					}
					passedDistance = 0;
				}
				//	DebugOut(L"passedDistance = %f . dy = %f . y = %f\n", passedDistance, dy, y);

			}
			else // Nếu Simon dừng trên cầu thang
			{
				if (this->directionY == -1) // Đang đi lên
					sprite->SelectFrame(SIMON_ANI_STAIR_STANDING_UP);
				else
					sprite->SelectFrame(SIMON_ANI_STAIR_STANDING_DOWN);
			}
		}

		//	DebugOut(L"sprite index = %d \n", sprite->GetCurrentFrame());

	}

#pragma endregion

#pragma region Xử lý khi Simon không ở trên cầu thang

	else
	{
	if (isSitting == true)
	{
		if (isAttacking == true) // Nếu Simon đang ở trạng thái tấn công
		{
			/* Xử lí ani ngồi đánh */
			if (index < SIMON_ANI_SITTING_ATTACKING_BEGIN) // Nếu ani chưa đúng
			{
				sprite->SelectFrame(SIMON_ANI_SITTING_ATTACKING_BEGIN); // Set lại ani bắt đầu
				sprite->timeAccumulated = dt;
			}
			else
			{
				/* Update ani bình thường */
				sprite->timeAccumulated += dt;
				if (sprite->timeAccumulated >= SIMON_TIME_WAIT_ANI_ATTACKING)
				{
					sprite->timeAccumulated -= SIMON_TIME_WAIT_ANI_ATTACKING;
					sprite->SelectFrame(sprite->GetCurrentFrame() + 1);
				}
				/* Update ani bình thường */

				if (sprite->GetCurrentFrame() > SIMON_ANI_SITTING_ATTACKING_END) // đã đi vượt qua frame cuối
				{
					isAttacking = false;
					sprite->SelectFrame(SIMON_ANI_SITTING);
				}
			}
			/* Xử lí ani ngồi đánh */
			DebugOut(L"update ani simon dt = %d, tich luy = %d\n", dt, sprite->timeAccumulated);

		}
		else
		{
			sprite->SelectFrame(SIMON_ANI_SITTING);
		}

	}
	else
	{
		if (isAttacking == true)
		{

			/* Xử lí ani đứng đánh */
			if (index < SIMON_ANI_STANDING_ATTACKING_BEGIN) // nếu ani chưa đúng
			{
				sprite->SelectFrame(SIMON_ANI_STANDING_ATTACKING_BEGIN); // set lại ani bắt đầu
				sprite->timeAccumulated = dt;
			}
			else
			{
				/* Update ani bình thường */
				sprite->timeAccumulated += dt;
				if (sprite->timeAccumulated >= SIMON_TIME_WAIT_ANI_ATTACKING)
				{
					sprite->timeAccumulated -= SIMON_TIME_WAIT_ANI_ATTACKING;
					sprite->SelectFrame(sprite->GetCurrentFrame() + 1);
				}
				/* Update ani bình thường */

				if (sprite->GetCurrentFrame() > SIMON_ANI_STANDING_ATTACKING_END) // đã đi vượt qua frame cuối
				{
					//mapWeapon[TypeWeaponCollect]->Start(this->x, this->y);

					isAttacking = false;
					sprite->SelectFrame(SIMON_ANI_IDLE);
				}
			}
			/* Xử lí ani đứng đánh */

		}
		else
		{
			if (isWalking == true) // đang di chuyển
			{
				if (isJumping == false) // ko nhảy
				{
					if (index < SIMON_ANI_WALKING_BEGIN || index >= SIMON_ANI_WALKING_END)
						sprite->SelectFrame(SIMON_ANI_WALKING_BEGIN);

					//cập nhật frame mới
					sprite->Update(dt); // dt này được cập nhật khi gọi update; 
				}
				else
				{
					sprite->SelectFrame(SIMON_ANI_JUMPING);
				}

			}
			else
			{
				if (isJumping) // nếu ko đi mà chỉ nhảy
				{
					sprite->SelectFrame(SIMON_ANI_JUMPING);
				}
				else
				{
					sprite->SelectFrame(SIMON_ANI_IDLE);		// SIMON đứng yên

				}
			}
		}
	}
 }
	

#pragma endregion

	GameObject::Update(dt);

	// Xét xem Simon đang trong trạng thái nhảy hay mới vô game
	if (isOnStair == false) // Ko trên cầu thang thì mới có trọng lực
	{
		if (isJumping)
		{
			dx = vx * dt;
			dy = vy * dt;
			vy += SIMON_GRAVITY_JUMPING * dt;
		}
		else
		{
			/*if (isHurting)
			{
				vy += SIMON_GRAVITY_HURTING * dt;
			}
			else*/
			vy += SIMON_GRAVITY * dt;// Simple fall down
		}
	}


	// Nếu không trên cầu thang thì xét xem Simon có đang tự đi hay không, nếu có thì update x, y cho Simon, ngược lại thì xét va chạm gạch
	if (isOnStair == false)
	{
		if (isAutoGoX == false)
			CollisionWithBrick(coObjects);
		else
			x += dx;
	}

	if (isOnStair == true)
		CollisionWithStair(coObjects);

	for (auto& objWeapon : mapWeapon)
	{
		if (objWeapon.second->GetFinish() == false) // Vũ khi chưa kết thúc thì update
		{
			if (objWeapon.second->GetType() == TAG::MORNINGSTAR)
			{
				objWeapon.second->SetPosition(this->x, this->y);
				objWeapon.second->SetSpeed(vx, vy); // set vận tốc để kt va chạm
				objWeapon.second->UpdatePositionFitSimon();
			}

			objWeapon.second->Update(dt, coObjects);
		}
	}

	if (isProcessingOnStair == 3)
	{
		isProcessingOnStair = 0;
		vx = 0;
		vy = 0;
		isWalking = false;
	}

	if (isAutoGoX == true)
	{
		if (abs(x - AutoGoX_Backup_X) >= AutoGoX_Distance)
		{
			x = x - (abs(x - AutoGoX_Backup_X) - AutoGoX_Distance);
			RestoreBackupAutoGoX();
			isAutoGoX = false;

			DebugOut(L"[SIMON] end auto go X\n");
		}
	}
}

#pragma region Các hàm xử lý va chạm

void Simon::CollisionWithBrick(const vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<LPGAMEOBJECT> list_Brick;
	list_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == TAG::BRICK)
			list_Brick.push_back(coObjects->at(i));

	CalcPotentialCollisions(&list_Brick, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		isCollisionAxisYWithBrick = false; // đang ko va chạm trục y
	//	DebugOut(L"%d : Col y = false (size = 0) - dt = %d - y = %f - dy = %f\n",GetTickCount(),dt,y, dy);
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;

		if (ny == -1)
			y += min_ty * dy + ny * 0.4f;
		else
			y += dy;

		if (ny == -1)
		{
			vy = 0.1f;
			dy = vy * dt;

			if (isJumping)
			{
				isJumping = false;
				y = y - PULL_UP_SIMON_AFTER_JUMPING;
			}
		}


		if (ny != 0)
		{
			isCollisionAxisYWithBrick = true;
			//		DebugOut(L"%d : Col y = true - dt=%d - y = %f - dy = %f\n", GetTickCount(), dt,y, dy);
		}
		else
		{
			//	DebugOut(L"%d : Col y = false - dt=%d\n", GetTickCount(), dt);
			isCollisionAxisYWithBrick = false;// đang ko va chạm trục y
		}



		/*if (nx != 0 || ny != 0)
		{
			isHurting = 0;
		}*/

	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

bool Simon::isCollisionWithItem(Item* objItem)
{
	if (objItem->GetFinish() == true)
		return false;

	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);  // lấy BBOX của simon

	objItem->GetBoundingBox(l1, t1, r1, b1);
	if (Game::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1) == true)
	{
		return true; // check with AABB
	}

	return false;
}

void Simon::CollisionWithStair(vector<LPGAMEOBJECT>* coObjects)
{
	if (directionY == 1) // đang đi xuống
	{
		int CountCollisionBottom = 0; // Biến cờ để check Simon có va chạm với nền hay chưa
		vector<LPGAMEOBJECT> listobj;
		listobj.clear();
		for (UINT i = 0; i < (*coObjects).size(); i++)
			if ((*coObjects)[i]->GetType() == TAG::STAIR_BOTTOM) // nếu là object ở dưới
			{
				if (this->isCollitionObjectWithObject((*coObjects)[i]))
				{
					CountCollisionBottom++;
					break;
				}
			}

		if (CountCollisionBottom > 0) // Có va chạm với bottom
		{
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;
			coEvents.clear();
			vector<LPGAMEOBJECT> list_Brick;
			list_Brick.clear();
			for (UINT i = 0; i < coObjects->size(); i++)
				if (coObjects->at(i)->GetType() == TAG::BRICK)
					list_Brick.push_back(coObjects->at(i));
			CalcPotentialCollisions(&list_Brick, coEvents);
			if (coEvents.size() == 0)
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (nx != 0 || ny != 0)
				{
					vx = 0;
					vy = 0;
					isOnStair = false; // kết thúc việc đang trên cầu thang
					isWalking = false;
					isProcessingOnStair = 0;
				}
			}

			for (UINT i = 0; i < coEvents.size(); i++)
				delete coEvents[i];

			return;
		}

	}

	if (directionY == -1) // đang đi lên
	{
		vector<LPGAMEOBJECT> listobj;
		int CountCollisionTop = 0;
		listobj.clear();
		for (UINT i = 0; i < (*coObjects).size(); i++)
			if ((*coObjects)[i]->GetType() == TAG::STAIR_TOP) // nếu là object ở trên
			{
				if (this->isCollitionObjectWithObject((*coObjects)[i])) // có va chạm với top stair
				{
					CountCollisionTop++;
					break;
				}
			}

		if (CountCollisionTop > 0) // có va chạm với top, và nó đang đi lên
		{
			float backupVy = vy;

			y = y - 50; // kéo simon lên cao, để tạo va chạm giả xuống mặt đất, tránh overlaping. tính thời gian tiếp đất
			vy = 9999999999.0f; // vận tốc kéo xuống lớn để chạm đất ngay trong 1 frame
			dy = vy * dt; // cập nhật lại dy

			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;
			coEvents.clear();
			vector<LPGAMEOBJECT> list_Brick;
			list_Brick.clear();

			for (UINT i = 0; i < coObjects->size(); i++)
				if (coObjects->at(i)->GetType() == TAG::BRICK)
					list_Brick.push_back(coObjects->at(i));

			CalcPotentialCollisions(&list_Brick, coEvents);
			if (coEvents.size() == 0)
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;

				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (nx != 0 || ny != 0)
				{
					vx = 0;
					vy = 0;
					isOnStair = false; // kết thúc việc đang trên cầu thang
					isWalking = false;
					isProcessingOnStair = 0;
				}
			}

			for (UINT i = 0; i < coEvents.size(); i++)
				delete coEvents[i];

			vy = backupVy;
			dy = vy * dt; // cập nhật lại dy

			return; // ko cần xét tiếp
		}
	}

	// nếu không đụng top và bot thì di chuyển bt
	x += dx;
	y += dy;
}

#pragma endregion



void Simon::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

	int alpha = 255;// độ trong suốt

	D3DXVECTOR2 pos = camera->Transform(x, y);

	if (this->GetFreeze() == true)
	{
		if (direction == -1)
			sprite->Draw(pos.x, pos.y, alpha, rand() % 256, rand() % 256, rand() % 256);
		else
			sprite->DrawFlipX(pos.x, pos.y, alpha, rand() % 256, rand() % 256, rand() % 256);

	}
	else
	{
		if (direction == -1)
			sprite->Draw(pos.x, pos.y, alpha);
		else
			sprite->DrawFlipX(pos.x, pos.y, alpha);
	}

	for (auto& objWeapon : mapWeapon)
	{
		if (objWeapon.second->GetFinish() == false) // Vũ khi này chưa kết thúc thì render
		{
			objWeapon.second->Render(camera);
		}
	}
}

void Simon::Sit()
{
	if (isOnStair == true)
		return;

	vx = 0;
	isWalking = 0;

	if (isSitting == false)
		y = y + PULL_UP_SIMON_AFTER_SITTING;

	isSitting = 1;
}

void Simon::ResetSit()
{
	if (isSitting == true)
	{
		isSitting = 0;
		y = y - PULL_UP_SIMON_AFTER_SITTING;
	}
}

void Simon::Left()
{
	if (isOnStair == true)
		return;

	direction = -1;
}

void Simon::Right()
{
	if (isOnStair == true)
		return;

	direction = 1;
}

void Simon::Go()
{
	if (isOnStair == true)
		return;

	if (isAttacking == true)
		return;

	vx = SIMON_WALKING_SPEED * direction;
	isWalking = 1;
}

void Simon::Jump()
{
	if (isJumping)
		return;

	if (isOnStair)
		return;

	if (isSitting)
		return;

	if (isAttacking)
		return;

	/*if (isHurting)
		return;*/

	vy = -SIMON_VJUMP;
	isJumping = true;
}

void Simon::Stop()
{
	if (isAttacking == true)
		return;

	if (isOnStair)
		return;

	/*if (isHurting)
		return;*/

	vx = 0;
	//DebugOut(L"[STOP] Set vx = %f \n", vx);


	isWalking = 0;
	if (isSitting == true)
	{
		isSitting = 0;
		y = y - PULL_UP_SIMON_AFTER_SITTING;
	}
}

void Simon::Attack(TAG weaponType)
{
	if (weaponType == TAG::NO_SUBWEAPON) // không có vũ khí phụ thì không thực hiện
		return;
	

	/* Kiểm tra còn đủ HeartCollect ko?, nếu ko đủ thì sẽ ko thể tấn công bằng vũ khí phụ */
	switch (weaponType)
	{
	case MORNINGSTAR:
	{
		if (isAttacking)
		{
			return;
		}
		break;
	}

	case STOPWATCH:
	{
		if (HeartCollect < 5) // Ko đủ HeartCollect thì ko thể attack
			return;
	}

	default: // Các vũ khí còn lại
	{
		if (HeartCollect < 1)
			return; // Ko đủ HeartCollect thì ko thể attack
	}

	}

	bool isAllowSubHeartCollect = false;

	if (mapWeapon[weaponType]->GetFinish()) { // Vũ khí đã kết thúc tấn công thì mới đc tấn công tiếp

		isAttacking = 1;
		sprite->SelectFrame(0);
		sprite->ResetAccumulatedTime();

		mapWeapon[weaponType]->Attack(this->x, this->y, this->direction); // Tấn công với vũ khí phụ Simon đang sở hữu
		isAllowSubHeartCollect = true;
	}
	else // Phần xử lí Double Shot
	{
		// Đang ở chế độ DoubleShot và vũ khi Simon dùng ko phải là Morning Star và Stop Watch
		if (isUsingDoubleShot && weaponType != TAG::MORNINGSTAR && weaponType != TAG::STOPWATCH)
		{
			// Sau 250 ms thì mới được dùng lại Double Shot, là sau khi vũ khí đầu tiên đã tấn công và chưa kết thúc do mới đầu sẽ rơi vào if ở trên và attack ra 1 vũ khí trc
			if (GetTickCount() - mapWeapon[weaponType]->GetLastTimeAttack() >= 250) 
			{
				bool isMustRecreateDoubleShot = false; // Ban đầu ko cần tạo lại do chưa sử dụng


				if (mapWeapon.find(TAG::WEAPON_DOUBLE_SHOT) == mapWeapon.end()) // Chưa khởi tạo Double Shot
				{
					isMustRecreateDoubleShot = true; // Chưa tạo thì phải tạo lại
				}
				else
				{
					if (mapWeapon[TAG::WEAPON_DOUBLE_SHOT]->GetFinish() == false) // Vũ khí đã tạo vẫn còn đang tấn công
					{
						return; // Thoát và đợi đến khi tấn công hoàn thành
					}
					else
					{
						// Vũ khí đã tạo khác với vũ khí đang dùng để tấn công, trường hợp Simon nhặt dc vũ khí khác
						if (mapWeapon[TAG::WEAPON_DOUBLE_SHOT]->GetType() != weaponType)
						{
							isMustRecreateDoubleShot = true; // Tạo lại cho đúng
						}
					}
				}

				if (isMustRecreateDoubleShot) // Tạo vũ khí DoubleShot
				{
					SAFE_DELETE(mapWeapon[TAG::WEAPON_DOUBLE_SHOT]); // Xoá vũ khí hiện tại
					TAG subWeaponType = GetTypeSubWeapon(); // Lấy loại vũ khí phụ mà Simon nhặt dc

					switch (subWeaponType)
					{

					case DAGGER:
					{
						mapWeapon[TAG::WEAPON_DOUBLE_SHOT] = new Dagger(camera);
						break;
					}

					case HOLYWATER:
					{
						mapWeapon[TAG::WEAPON_DOUBLE_SHOT] = new HolyWater(camera);
						break;
					}

					case STOPWATCH:
					{
						mapWeapon[TAG::WEAPON_DOUBLE_SHOT] = new StopWatch();
						break;
					}

					case THROWINGAXE:
					{
						mapWeapon[TAG::WEAPON_DOUBLE_SHOT] = new ThrowingAxe(camera);
						break;
					}

					case BOOMERANG:
					{
						mapWeapon[TAG::WEAPON_DOUBLE_SHOT] = new Boomerang(camera, this);
						break;
					}
					default:
						break;
					}
				}


				isAttacking = true; // Set trang thái tấn công
				sprite->SelectFrame(0);
				sprite->ResetAccumulatedTime();


				mapWeapon[TAG::WEAPON_DOUBLE_SHOT]->Attack(this->x, this->y, this->direction);
				isAllowSubHeartCollect = true;
			}

		}
	}

	// Nếu vũ khí còn đủ heart để attack
	if (isAllowSubHeartCollect)
	{
		switch (weaponType)
		{
		case MORNINGSTAR:
		{
			// Ko trừ
			break;
		}

		case STOPWATCH:
		{
			HeartCollect -= 5;
			break;
		}

		default: // Các vũ khí còn lại
		{
			HeartCollect -= 1;
			break;
		}
		}
	}
}



#pragma region Trạng thái đóng băng

bool Simon::GetFreeze()
{
	return isFreeze;
}

void Simon::SetFreeze(int f)
{
	isFreeze = f;
	TimeFreeze = 0; // thời gian đã đóng băng
}

void Simon::UpdateFreeze(DWORD dt)
{
	if (TimeFreeze + dt >= TIME_FREEZE_MAX)
	{
		SetFreeze(false); // kết thúc đóng băng
	}
	else
		TimeFreeze += dt;
}

#pragma endregion



#pragma region Phần tự động đi của Simon

bool Simon::GetIsAutoGoX()
{
	return isAutoGoX;
}

void Simon::SetAutoGoX(int GoDirection, int DirectionAfterGo, float Distance, float WalkSpeed)
{
	// Nếu Simon đang tự đi thì ko xét
	if (isAutoGoX)
		return;

	isAutoGoX = true;

	AutoGoX_Backup_X = x; // Giữ lại vị trí trước khi đi tự động

	//Backup trạng thái
	isWalking_Backup = isWalking;
	isJumping_Backup = isJumping;
	isSitting_Backup = isSitting;
	isAttacking_Backup = isAttacking;
	isOnStair_Backup = isOnStair;
	isProcessingOnStair_Backup = isProcessingOnStair;
	directionStair_Backup = directionStair;
	directionY_Backup = directionY;
	//================================================//

	AutoGoX_Distance = Distance;
	AutoGoX_Speed = WalkSpeed;
	AutoGoX_GoDirection = GoDirection;
	directionAfterGo = DirectionAfterGo;

	direction = GoDirection;
	vx = WalkSpeed * GoDirection;
	isWalking = 1;
	isJumping = 0;
	isSitting = 0;
	isAttacking = 0;
	isOnStair = 0;
	isProcessingOnStair = 0;
}

void Simon::RestoreBackupAutoGoX()
{
	isWalking = isWalking_Backup;
	isJumping = isJumping_Backup;
	isSitting = isSitting_Backup;
	isAttacking = isAttacking_Backup;
	isOnStair = isOnStair_Backup;
	isProcessingOnStair = isProcessingOnStair_Backup;
	directionStair = directionStair_Backup;
	directionY = directionY_Backup;

	direction = directionAfterGo; // set hướng sau khi đi

	isWalking = 0; // tắt trạng thái đang đi
	isAutoGoX = 0; // tắt trạng thái auto

	vx = 0;
	vy = 0;
	// đi xong thì cho simon đứng yên
}

#pragma endregion



#pragma region Phần get set cho mạng, điểm và tim

int Simon::GetLives()
{
	return Lives;
}

void Simon::SetLives(int lives)
{
	Lives = lives;
}

int Simon::GetScore()
{
	return Score;
}

void Simon::SetScore(int score)
{
	Score = score;
}

void Simon::SetHeartCollect(int h)
{
	HeartCollect = h;
}

int Simon::GetHeartCollect()
{
	return HeartCollect;
}

#pragma endregion



#pragma region Các hàm xử lý nhặt vũ khí

void Simon::PickUpSubWeapon(TAG t)
{
	switch (t) {

	case DAGGER:
		if (mapWeapon[t] == NULL) { // chưa có thì thêm vào map
			mapWeapon[t] = new Dagger(camera);
		}
		break;

	case HOLYWATER:
	{
		if (mapWeapon[t] == NULL)
		{
			mapWeapon[t] = new HolyWater(camera);
		}
		break;
	}

	case THROWINGAXE:
	{
		if (mapWeapon[t] == NULL)
		{
			mapWeapon[t] = new ThrowingAxe(camera);
		}
		break;
	}

	case BOOMERANG:
	{
		if (mapWeapon[t] == NULL)
		{
			mapWeapon[t] = new Boomerang(camera, this);
		}
		break;
	}

	case STOPWATCH:
	{
		if (mapWeapon[t] == NULL)
		{
			mapWeapon[t] = new StopWatch();
		}
		break;
	}

	}

	SetTypeSubWeapon(t); // set loại vũ khí phụ hiện tại
}

TAG Simon::GetTypeSubWeapon()
{
	return SubWeaponType;
}

void Simon::SetTypeSubWeapon(TAG t)
{
	SubWeaponType = t;
}

bool Simon::GetIsUsingDoubleShot()
{
	return isUsingDoubleShot;
}

void Simon::SetIsUsingDoubleShot(bool IsUsingDoubleShot)
{
	isUsingDoubleShot = IsUsingDoubleShot;
}

bool Simon::IsUsingWeapon(TAG weaponType)
{
	if (this->mapWeapon.find(weaponType) != this->mapWeapon.end()) // Có tồn tại trong list weapon
	{
		if (this->mapWeapon[weaponType]->GetFinish() == false) // Chưa kết thúc, là Simon đang tấn công với vũ khí
			return true;
	}
	return false;
}

#pragma endregion