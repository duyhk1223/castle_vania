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
	this->y = y;
	this->direction = Direction;
	isFinish = false; // Mới tấn công

	LastTimeAttacked = GetTickCount(); // Lưu lại thời điểm tấn công
}

void Weapon::Render(Camera* camera)
{
	// Nếu đang ko tấn công thì ko render
	if (isFinish)
		return;

	D3DXVECTOR2 position = camera->Transform(x, y);
	if (direction == 1)
		sprite->Draw(position.x, position.y);
	else
		sprite->DrawFlipX(position.x, position.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}

void Weapon::UpdatePositionFitSimon()
{
}

bool Weapon::GetFinish()
{
	return isFinish;
}

void Weapon::SetFinish(bool IsFinish)
{
	this->isFinish = IsFinish;
}

DWORD Weapon::GetLastTimeAttacked()
{
	return LastTimeAttacked;
}