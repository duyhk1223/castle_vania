#include "MoneyBag.h"


MoneyBag::MoneyBag(float X, float Y, TAG TypeMoneyBag)
{
	type = TypeMoneyBag;
	texture = TextureManager::GetInstance()->GetTexture(TAG::BONUS);
	sprite = new Sprite(texture, 0);

	this->x = X;
	this->y = Y;

	vx = 0;

	vy = MONEYBAG_GRAVITY;
	MaxDisplayTime = MONEYBAG_TIMEDISPLAYMAX; // set time hiển thị tối đa
	DisplayedTime = 0;
	WaitedTime = 0;
	MaxWaitingTime = MONEYBAG_TIMEWAITMAX;
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

	D3DXVECTOR2 pos = camera->Transform(x, y);
	sprite->Draw(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}

MoneyBag::~MoneyBag()
{
}
