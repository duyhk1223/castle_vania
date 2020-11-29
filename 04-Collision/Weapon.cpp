#include "Weapon.h"

Weapon::Weapon()
{
	isFinish = 1;
}

Weapon::~Weapon()
{
}

int Weapon::GetDirection()
{
	return direction;
}

void Weapon::SetDirection(int Direction)
{
	this->direction = Direction;
}

void Weapon::Attack(float X, float Y, int Direction)
{
	this->x = X;
	this->y = Y;
	this->direction = Direction;
	isFinish = false; // Mới tấn công

	LastTimeAttack = GetTickCount(); // Lưu lại thời điểm tấn công, làm đánh dấu tránh 1 hit đánh nhiều lần cho các object, có health > 1.
}

void Weapon::Render(Camera* camera)
{
	// Nếu đang ko tấn công thì ko render
	if (isFinish)
		return;

	D3DXVECTOR2 pos = camera->Transform(x, y);
	if (direction == -1)
		sprite->Draw(pos.x, pos.y);
	else
		sprite->DrawFlipX(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}

void Weapon::UpdatePositionFitSimon()
{
}

bool Weapon::isCollision(GameObject* obj)
{
	if (isFinish == true)
		return false;

	// dt, dx, dy đã update 
	if (obj->GetHealth() <= 0) // đã chết
		return false;
	return isCollitionObjectWithObject(obj);
}

bool Weapon::GetFinish()
{
	return isFinish;
}

void Weapon::SetFinish(bool IsFinish)
{
	this->isFinish = IsFinish;
}

DWORD Weapon::GetLastTimeAttack()
{
	return LastTimeAttack;
}