#include "Bonus.h"

Bonus::Bonus(float X, float Y)
{
	type = TAG::BONUS;
	texture = TextureManager::GetInstance()->GetTexture(TAG::BONUS);
	sprite = new Sprite(texture, 20);

	this->x = X;
	this->y = Y;

	vx = 0;
	vy = 0;

	vy = BONUS_GRAVITY;

	// Set up time hiển thị cũng như chờ
	MaxDisplayTime = BONUS_TIMEDISPLAYMAX; 
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = BONUS_TIMEWAITMAX;
}

Bonus::~Bonus()
{
}

void Bonus::Render(Camera* camera)
{
	Item::Render(camera);
	sprite->Update(dt);
}