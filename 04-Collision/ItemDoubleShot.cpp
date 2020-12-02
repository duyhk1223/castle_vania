#include "ItemDoubleShot.h"



ItemDoubleShot::ItemDoubleShot(float X, float Y)
{
	type = TAG::ITEMDOUBLESHOT;

	texture = TextureManager::GetInstance()->GetTexture(TAG::ITEMDOUBLESHOT);
	sprite = new Sprite(texture, 100);

	x = X;
	y = Y;
	Health = 1;

	vy = ITEMDOUBLESHOT_GRAVITY;
	MaxDisplayTime = ITEMDOUBLESHOT_MAXDISPLAYTIME; // set time hiển thị tối đa
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = ITEMDOUBLESHOT_MAXWAITINGTIME;
}


ItemDoubleShot::~ItemDoubleShot()
{
}
