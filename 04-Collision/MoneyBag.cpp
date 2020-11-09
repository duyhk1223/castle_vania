#include "MoneyBag.h"

MoneyBag::MoneyBag(float X, float Y, TAG MoneyBagType)
{
	type = MoneyBagType;
	texture = TextureManager::GetInstance()->GetTexture(TAG::BONUS);
	sprite = new Sprite(texture, 0);

	this->x = X;
	this->y = Y;

	vy = MONEYBAG_GRAVITY;
	MaxDisplayTime = MONEYBAG_TIMEDISPLAYMAX;
	DisplayedTime = 0;
	MaxWaitingTime = MONEYBAG_TIMEWAITMAX;
	WaitedTime = 0;
}

void MoneyBag::Render(Camera* camera)
{
	if (IsWaitingToDisplay())
	{
		return;
	}

	switch (type)
	{
	case TAG::MONEY_RED_BAG:
		sprite->SelectFrame(MONEYBAG_ANI_RED_BAG);
		break;

	case TAG::MONEY_WHITE_BAG:
		sprite->SelectFrame(MONEYBAG_ANI_WHITE_BAG);
		break;

	case TAG::MONEY_PURPLE_BAG:
		sprite->SelectFrame(MONEYBAG_ANI_PURPLE_BAG);
		break;
	}
}

MoneyBag::~MoneyBag()
{
}