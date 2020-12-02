#include "ItemStopWatch.h"


ItemStopWatch::ItemStopWatch(float X, float Y)
{
	type = TAG::STOPWATCH;
	texture = TextureManager::GetInstance()->GetTexture(type);

	sprite = new Sprite(texture, 100);

	this->x = X;
	this->y = Y;
	vy = STOPWATCH_GRAVITY;
	MaxDisplayTime = STOPWATCH_MAXDISPLAYTIME; // set time hiển thị tối đa
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = STOPWATCH_MAXWAITINGTIME;
}

ItemStopWatch::~ItemStopWatch()
{
}
