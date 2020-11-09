#include "Item.h"

Item::Item()
{
	MaxDisplayTime = 0;
	MaxWaitingTime = 0;
	MaxDisplayTime = 0;
	WaitedTime = 0;
	isFinished = 0;
}

Item::~Item()
{
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects)
{
	if (IsWaitingToDisplay())
	{
		WaitedTime += dt;
		return;
	}

	// Nếu item đang hiển thị
	DisplayedTime += dt;
	if (DisplayedTime >= MaxDisplayTime)
	{
		isFinished = true;
		return;
	}

	GameObject::Update(dt);

	// Lấy ra các object gạch để xét va chạm khi item rới xuống gạch
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
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0)
		{
			vy = 0;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void Item::Render(Camera* camera)
{
	if (IsWaitingToDisplay())
		return;

	D3DXVECTOR2 position = camera->Transform(x, y);
	
	sprite->Draw(position.x, position.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}

bool Item::IsWaitingToDisplay()
{
	return WaitedTime < MaxWaitingTime;
}

bool Item::GetFinish()
{
	return isFinished;
}

void Item::SetFinish(bool IsFinished)
{
	this->isFinished = IsFinished;
}