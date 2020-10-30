#include "Simon.h"


Simon::Simon(Camera* camera)
{
	texture = TextureManager::GetInstance()->GetTexture(TAG::SIMON);
	sprite = new Sprite(texture, 250);
	//_sprite_deadth = new Sprite(TextureManager::GetInstance()->GetTexture(TAG::SIMON_DEADTH), 250);
	type = TAG::SIMON;


	this->camera = camera;
	//this->sound = Sound::GetInstance();
	//mapWeapon[TAG::MORNINGSTAR] = new MorningStar();

	Init();
}

Simon::~Simon()
{
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
	/* Không cho lọt khỏi camera */


#pragma region Update sprite

	int index = sprite->GetCurrentFrame();

	
	if (isSitting == true)
	{
		if (isAttacking == true) // tấn công
		{
			/* Xử lí ani ngồi đánh */
			if (index < SIMON_ANI_SITTING_ATTACKING_BEGIN) // nếu ani chưa đúng
			{
				sprite->SelectFrame(SIMON_ANI_SITTING_ATTACKING_BEGIN); // set lại ani bắt đầu
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
				if (isJumping == true) // nếu ko đi mà chỉ nhảy
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

#pragma endregion

	GameObject::Update(dt);

	if (isJumping)
	{
		dx = vx * dt;
		dy = vy * dt;
		vy += SIMON_GRAVITY_JUMPING * dt;
	}

	vy += SIMON_GRAVITY * dt;// Simple fall down

	// xét va chạm với brick
	CollisionWithBrick(coObjects); // check Collision and update x, y for simon

}

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

void Simon::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

	int alpha = 255;// độ trong suốt

	D3DXVECTOR2 pos = camera->Transform(x, y);

	if (direction == -1)
		sprite->Draw(pos.x, pos.y, alpha);
	else
		sprite->DrawFlipX(pos.x, pos.y, alpha);
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
	if (isJumping)
		return;

	/*if (isOnStair == true)
		return;*/

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

void Simon::Init()
{
	Reset();
}

void Simon::Reset()
{

	direction = 1;

	isSitting = 0;
	
	isJumping = 0;
	isWalking = 0;
	isAttacking = 0;

	

	vx = 0;
	vy = 0;
	
}