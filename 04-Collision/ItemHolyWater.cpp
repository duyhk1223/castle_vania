#include "ItemHolyWater.h"





ItemHolyWater::ItemHolyWater(float X, float Y)
{
	texture = TextureManager::GetInstance()->GetTexture(TAG::ITEMHOLYWATER);
	sprite = new Sprite(texture, 0);
	type = TAG::ITEMHOLYWATER;

	this->x = X;
	this->y = Y;
	vy = ITEMHOLYWATER_GRAVITY;
	MaxDisplayTime = ITEMHOLYWATER_MAXDISPLAYTIME;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = ITEMHOLYWATER_MAXWAITINGTIME;
}

ItemHolyWater::~ItemHolyWater()
{
}
