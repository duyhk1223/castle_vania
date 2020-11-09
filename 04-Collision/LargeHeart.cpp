#include "LargeHeart.h"



LargeHeart::LargeHeart(float X, float Y)
{
	type = TAG::LARGEHEART;

	texture = TextureManager::GetInstance()->GetTexture(type);

	sprite = new Sprite(texture, 100);

	this->x = X;
	this->y = Y;
	vy = LARGEHEART_GRAVITY;

	// Set up time hiển thị cũng như chờ
	MaxDisplayTime = LARGEHEART_TIMEDISPLAYMAX;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = LARGEHEART_TIMEWAITMAX;
}


LargeHeart::~LargeHeart()
{
}
