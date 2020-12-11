#include "CrystalBall.h"

CrystalBall::CrystalBall(float X, float Y)
{
	type = TAG::CRYSTALBALL;
	texture = TextureManager::GetInstance()->GetTexture(TAG::CRYSTALBALL);
	sprite = new Sprite(texture, 50);

	this->x = X;
	this->y = Y;
	Health = 1;

	vy = CRYSTALBALL_GRAVITY;
	WaitedTime = 0;
	MaxWaitingTime = CRYSTALBALL_MAXWAITINGTIME;
}

CrystalBall::~CrystalBall()
{
}

void CrystalBall::Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects)
{
	if (IsWaitingToDisplay()) // Chưa đủ thời gian để dc hiển thị thì sẽ đợi tiếp
	{
		WaitedTime += dt;
		return;
	}

	GameObject::Update(dt, listObjects); // Update dx, dy, dt



#pragma region Phần xử lý va chạm gạch

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
	if (coEvents.size() == 0) // Chưa có va chạm thì tiếp tục rơi
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

#pragma endregion


	sprite->Update(dt);
}

void CrystalBall::Render(Camera* camera)
{
	if (IsWaitingToDisplay()) // Chưa hiển thị ra thì ko xét
		return;

	//sprite->Update(dt);

	D3DXVECTOR2 pos = camera->Transform(x, y);

	sprite->Draw(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}