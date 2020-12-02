#include "BlackPanther.h"

BlackPanther::BlackPanther(float X, float Y, int Direction, float AutoGoX_Distance, Simon* simon)
{
	type = TAG::PANTHER;
	Health = 1;
	vx = vy = 0;
	direction = Direction;
	this->x = X;
	this->y = Y;
	AutoGoX_Backup = x;
	this->AutoGoX_Distance = AutoGoX_Distance;

	texture = TextureManager::GetInstance()->GetTexture(TAG::PANTHER);
	sprite = new Sprite(texture, 120);

	isSitting = 1; // Mới đầu thì báo sẽ ngồi
	isRunning = 0;
	isJumping = 0;
	isStart = 0;
	isAutoGoX = 0;

	this->simon = simon;
}

void BlackPanther::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	// Nếu báo ko ở trạng thái nhảy thì sẽ rơi xuống
	if (isJumping)
	{
		dx = vx * dt;
		dy = vy * dt;
		vy += PANTHER_GRAVITY_JUMPING * dt;
	}
	else
		vy += PANTHER_GRAVITY * dt; // Simple fall down

	// Xét hướng của báo chạy để giới hạn biên chạy
	float LimitDistance;
	if (direction == 1) // Báo chạy qua phải
		LimitDistance = PANTHER_DEFAULT_DISTANCE_AUTO_GO_X_DIRECTION_RIGHT;
	else // Báo chạy qua trái
		LimitDistance = PANTHER_DEFAULT_DISTANCE_AUTO_GO_X_DIRECTION_LEFT;

	// Nếu Simon đang ở trong vùng kích hoạt báo
	if (isStart == 0 && abs(simon->GetX() - (x + texture->GetFrameWidth())) <= LimitDistance)
	{
		isSitting = false;
		isRunning = true;
		// Chuyển qua trạng thái chạy
		Run();

		isStart = 1;
		isAutoGoX = 1; // Bật trạng thái tự chạy
	}

	// Nếu đang ngồi thì sẽ chọn frame ngồi, nhảy thì sẽ chọn frame đứng yên và ko nhảy thì sẽ chọn frame đang chạy
	if (isSitting)
	{
		sprite->SelectFrame(PANTHER_ANI_SITTING);
	}
	else
	{
		if (isRunning) // Ko nhảy thì ko chọn frame nên sẽ là frame đứng yên
		{
			if (!isJumping) // Ko đang nhảy thì chạy => đang nhảy thì cho animation đứng yên
			{
				// Nếu frame đang nằm giữa khoảng frame của animation thì update tiếp
				if (PANTHER_ANI_RUNNING_BEGIN <= sprite->GetCurrentFrame() && sprite->GetCurrentFrame() < PANTHER_ANI_RUNNING_END)
				{
					sprite->Update(dt);
				}
				else // Nếu xong animation thì reset lại frame đầu
					sprite->SelectFrame(PANTHER_ANI_RUNNING_BEGIN);
			}

		}
	}


#pragma region Xét va chạm đất

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	vector<LPGAMEOBJECT> list_Brick;
	list_Brick.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == TAG::BRICK)
			list_Brick.push_back(coObjects->at(i));
	}

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

		x += dx;
		if (ny == -1)
			y += min_ty * dy + ny * 0.4f;
		else
			y += dy;

		if (ny == -1)
		{
			vy = 0;
			if (isJumping)
			{
				isJumping = false; // Kết thúc nhảy
				if (x < simon->GetX()) // Simon đang qua bên phải
				{
					direction = 1; // Đổi hướng panther qua phải 
				}
				else
				{
					direction = -1; // Đổi hướng panther qua trái
				}
				Run();
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

#pragma endregion

	// Nếu báo đang ở chế độ tự chạy
	if (isAutoGoX == true)
	{
		if (abs(x - AutoGoX_Backup) >= AutoGoX_Distance) // Nếu chạy đủ khoảng cách tự chạy tính từ vị trí ban đầu (Backup)
		{
			x = x - (abs(x - AutoGoX_Backup) - AutoGoX_Distance);
			isAutoGoX = false;
			vx = 0;

			Jump(); // Sau khi chạy xong thì nhảy

			DebugOut(L"[PANTHER] end auto go X\n");
		}
	}
}

void BlackPanther::Render(Camera* camera)
{
	if (Health <= 0)
		return;

	D3DXVECTOR2 pos = camera->Transform(x, y);
	if (direction == -1)
		sprite->Draw(pos.x, pos.y);
	else
		sprite->DrawFlipX(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}


bool BlackPanther::GetIsStart()
{
	return isStart;
}


void BlackPanther::Jump()
{
	// Nếu đang nhảy thì ko xét
	if (isJumping == true)
		return;

	vy = -PANTHER_VYJUMP; // Bắt đầu rơi
	vx = PANTHER_VXJUMP * direction; // Nhảy theo 1 hướng
	isJumping = true;

}


void BlackPanther::Run()
{
	vx = PANTHER_SPEED_RUNNING * direction;
	isRunning = true;
}


BlackPanther::~BlackPanther()
{
}