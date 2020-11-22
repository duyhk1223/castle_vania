#include "ItemDagger.h"

ItemDagger::ItemDagger(float X, float Y)
{
	type = TAG::ITEMDAGGER;
	texture = TextureManager::GetInstance()->GetTexture(TAG::ITEMDAGGER);
	sprite = new Sprite(texture, 0);

	this->x = X; 
	this->y = Y;

	vy = ITEMDAGGER_GRAVITY;
	MaxDisplayTime = ITEMDAGGER_TIMEDISPLAYMAX;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = ITEMDAGGER_TIMEWAITMAX;
}

ItemDagger::~ItemDagger()
{

}