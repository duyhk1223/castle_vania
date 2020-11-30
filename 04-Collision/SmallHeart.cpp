#include "SmallHeart.h"

SmallHeart::SmallHeart(float X, float Y)
{
	texture = TextureManager::GetInstance()->GetTexture(TAG::SMALLHEART);
	sprite = new Sprite(texture, 100);
	type = TAG::SMALLHEART;

	this->x = X;
	this->y = Y;
	this->xBackup = X;

	MaxDisplayTime = SMALLHEART_MAXDISPLAYTIME;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = SMALLHEART_MAXWAITINGTIME;

	vx = SMALLHEART_SPEED_X;
	vy = SMALLHEART_GRAVITY;
}

void SmallHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects)
{
	if (WaitedTime < MaxWaitingTime)
	{
		WaitedTime += dt;
		return;
	}

	DisplayedTime += dt;
	if (DisplayedTime >= MaxDisplayTime)
	{
		isFinished = true;
	}

	// Vượt quá biên độ dao động của trục x cho phép thì đổi chiều vx
	if (x - xBackup > DeltaX)
	{
		vx = -SMALLHEART_SPEED_X;
	}
	else 
	{
		if (x - xBackup <= -DeltaX)
		{
			vx = SMALLHEART_SPEED_X;
		}
	}

	GameObject::Update(dt, listObjects); // Update dx, dy, dt

#pragma region Phần xét va chạm với gạch

	vector<LPGAMEOBJECT> listObject_Brick;
	listObject_Brick.clear();
	for (UINT i = 0; i < listObjects->size(); i++)
		if (listObjects->at(i)->GetType() == TAG::BRICK)
			listObject_Brick.push_back(listObjects->at(i));

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&listObject_Brick, coEvents); // Lấy danh sách các va chạm

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		y += min_ty * dy + ny * 0.4f;

		if (ny != 0)
		{
			vy = 0;
			vx = 0; // cho dừng di chuyển theo biên độ x
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

#pragma end region
}

SmallHeart::~SmallHeart()
{
}