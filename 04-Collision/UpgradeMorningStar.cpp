#include "UpgradeMorningStar.h"

UpgradeMorningStar::UpgradeMorningStar(float X, float Y)
{
	type = TAG::UPGRADEMORNINGSTAR;
	texture = TextureManager::GetInstance()->GetTexture(TAG::UPGRADEMORNINGSTAR);
	sprite = new Sprite(texture, 0);

	this->x = X;
	this->y = Y;

	vy = UPGRADEMORNINGSTAR_GRAVITY;
	MaxDisplayTime = UPGRADEMORNINGSTAR_TIMEDISPLAYMAX;
	DisplayedTime = 0;
	MaxWaitingTime = UPGRADEMORNINGSTAR_TIMEWAITMAX;
	WaitedTime = 0;
}

UpgradeMorningStar::~UpgradeMorningStar()
{
}