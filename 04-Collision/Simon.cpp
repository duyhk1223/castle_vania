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

	isAutoGoX = 0;
	passedDistance = 0;

	isFreeze = 0;
	TimeFreeze = 0;

	vx = 0;
	vy = 0;

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
		if (isAttacking == true) // Simon tấn công khi lên thang
		{
			if (directionY == -1) // đang đi lên
			{
				/* Xử lí ani đánh khi đang đi lên thang*/

				if (index < SIMON_ANI_STAIR_UP_ATTACKING_BEGIN) // Nếu ani chưa đúng
				{
					sprite->SelectFrame(SIMON_ANI_STAIR_UP_ATTACKING_BEGIN); // Set lại ani bắt đầu
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
				if (index < SIMON_ANI_STAIR_DOWN_ATTACKING_BEGIN) // Nếu ani chưa đúng
				{
					sprite->SelectFrame(SIMON_ANI_STAIR_DOWN_ATTACKING_BEGIN); // Set lại ani bắt đầu
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
				if (isProcessingOnStair == 1) // nếu ở giai đoạn bước chân thì set frame 12
				{
					if (vy < 0) // ddi len
						sprite->SelectFrame(SIMON_ANI_STAIR_GO_UP_BEGIN);
					else
						sprite->SelectFrame(SIMON_ANI_STAIR_GO_DOWN_BEGIN);
				}


				if (isProcessingOnStair == 2) // nếu ở giai đoạn bước chân trụ thì set frame 13
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

					/* fix lỗi mỗi lần đi vượt quá 16px */
					if (direction == 1 && directionY == -1) // đi lên bên phải
					{
						x -= (passedDistance - 16.0f);
						y += (passedDistance - 16.0f);
					}
					if (direction == -1 && directionY == -1) // đi lên bên trái
					{
						x += (passedDistance - 16.0f);
						y += (passedDistance - 16.0f);
					}

					if (direction == 1 && directionY == 1) // đi xuống bên phải
					{
						x -= (passedDistance - 16.0f);
						y -= (passedDistance - 16.0f);
					}
					if (direction == -1 && directionY == 1) // đi xuống bên trái
					{
						x += (passedDistance - 16.0f);
						y -= (passedDistance - 16.0f);
					}
					passedDistance = 0;
				}
				//	DebugOut(L"DoCaoDiDuoc = %f . dy = %f . y = %f\n", DoCaoDiDuoc, dy, y);

			}
			else
			{
				if (this->directionY == -1) // ddang di len
					sprite->SelectFrame(SIMON_ANI_STAIR_STANDING_UP);
				else
					sprite->SelectFrame(SIMON_ANI_STAIR_STANDING_DOWN);
			}
		}
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

	// xét va chạm với brick
	CollisionWithBrick(coObjects); // check Collision và update x, y cho simon	

	// Nếu không trên cầu thang thì xét xem Simon có đang tự đi hay không, nếu có thì update x, y cho Simon, ngược lại thì xét va chạm gạch
	/*if (isOnStair == false)
	{
		if (isAutoGoX == false)
			CollisionWithBrick(coObjects);
		else
			x += dx;
	}*/

	if (isOnStair == true)
		CollisionWithStair(coObjects);

	if (isProcessingOnStair == 3)
	{
		isProcessingOnStair = 0;
		vx = 0;
		vy = 0;
		isWalking = false;
	}

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
		if (objWeapon.second->GetFinish() == false) // vũ khi này chưa kết thúc thì render
		{
			objWeapon.second->Render(camera);
		}
	}
}

void Simon::Sit()
{
	/*if (isOnStair == true)
		return;*/

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
	direction = -1;
}

void Simon::Right()
{
	direction = 1;
}

void Simon::Go()
{
	if (isAttacking == true)
		return;

	vx = SIMON_WALKING_SPEED * direction;
	isWalking = 1;

}

void Simon::Jump()
{
	if (isJumping == true)
		return;

	/*if (isOnStair == true)
		return;*/

	if (isSitting == true)
		return;

	if (isAttacking == true)
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

	/*if (isOnStair)
		return;

	if (isHurting)
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
	

	/* Kiểm tra còn đủ HeartCollect ko? */
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

	default: // các vũ khí còn lại
	{
		if (HeartCollect < 1)
			return;// ko đủ HeartCollect thì ko attack
		break;
	}
	}

	bool isAllowSubHeartCollect = false;

	if (mapWeapon[weaponType]->GetFinish()) { // vũ khí đã kết thúc thì mới đc tấn công tiếp

		isAttacking = 1;
		sprite->SelectFrame(0);
		sprite->ResetAccumulatedTime();

		mapWeapon[weaponType]->Attack(this->x, this->y, this->direction); // Render vũ khí của Simon đang sở hữu
		isAllowSubHeartCollect = true;
	}

	// Nếu vũ khí còn đủ heart để attack
	if (isAllowSubHeartCollect)
	{
		switch (weaponType)
		{
		case MORNINGSTAR:
		{
			// ko trừ
			break;
		}

		default: // các vũ khí còn lại
		{
			HeartCollect -= 1;
			break;
		}
		}
	}
}

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

	AutoGoX_Distance = Distance;
	AutoGoX_Speed = WalkSpeed;
	AutoGoX_GoDirection = GoDirection;
	directionAfterGo = DirectionAfterGo;

	direction = GoDirection;

	isWalking = 1;
	isJumping = 0;
	isSitting = 0;
	isAttacking = 0;
	isOnStair = 0;
	isProcessingOnStair = 0;
}

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

#pragma endregion