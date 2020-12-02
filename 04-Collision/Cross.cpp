#include "Cross.h"

Cross::Cross(float X, float Y)
{
	type = TAG::CROSS;
	texture = TextureManager::GetInstance()->GetTexture(TAG::CROSS);
	sprite = new Sprite(texture, 0);

	this->x = X;
	this->y = Y;
	vy = CROSS_GRAVITY;
	MaxDisplayTime = CROSS_MAXDISPLAYIME;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = CROSS_MAXWAITINGTIME;
}

Cross::~Cross()
{
}