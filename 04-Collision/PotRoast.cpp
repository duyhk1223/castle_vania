#include "PotRoast.h"

PotRoast::PotRoast(float X, float Y)
{
	type = TAG::POTROAST;
	texture = TextureManager::GetInstance()->GetTexture(TAG::POTROAST);
	sprite = new Sprite(texture, 100);

	this->x = X;
	this->y = Y;
	Health = 1;

	vy = POTROAST_GRAVITY;
	MaxDisplayTime = POTROAST_MAXDISPLAYTIME;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = POTROAST_MAXWAITINGTIME;
}

PotRoast::~PotRoast()
{
}