#include "ItemThrowingAxe.h"

ItemThrowingAxe::ItemThrowingAxe(float X, float Y)
{
	type = TAG::ITEMTHROWINGAXE;
	texture = TextureManager::GetInstance()->GetTexture(TAG::ITEMTHROWINGAXE);
	sprite = new Sprite(texture, 0);

	this->x = X;
	this->y = Y;
	vy = ITEMTHROWINGAXE_GRAVITY;
	MaxDisplayTime = ITEMTHROWINGAXE_MAXDISPLAYTIME;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = ITEMTHROWINGAXE_MAXWAITINGTIME;
}

ItemThrowingAxe::~ItemThrowingAxe()
{
}