#include "InvisibilityPotion.h"

InvisibilityPotion::InvisibilityPotion(float X, float Y)
{
	type = TAG::INVISIBILITYPOTION;
	texture = TextureManager::GetInstance()->GetTexture(TAG::INVISIBILITYPOTION);
	sprite = new Sprite(texture, 0);

	this->x = X;
	this->y = Y;

	vy = INVISIBILITYPOTION_GRAVITY;
	MaxDisplayTime = INVISIBILITYPOTION_MAXDISPLAYTIME;
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = INVISIBILITYPOTION_MAXWAITINGTIME;
}

InvisibilityPotion::~InvisibilityPotion()
{
}