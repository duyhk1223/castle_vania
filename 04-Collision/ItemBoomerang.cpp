#include "ItemBoomerang.h"



ItemBoomerang::ItemBoomerang(float X, float Y)
{
	type = TAG::ITEMBOOMERANG;

	texture = TextureManager::GetInstance()->GetTexture(type);
	sprite = new Sprite(texture, 100);

	x = X;
	y = Y;
	Health = 1;

	vy = ITEMBOOMERANG_GRAVITY;
	MaxDisplayTime = ITEMBOOMERANG_MAXDISPLAYTIME; // Set time hiển thị tối đa
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = ITEMBOOMERANG_MAXWAITINGTIME;
}


ItemBoomerang::~ItemBoomerang()
{
}
