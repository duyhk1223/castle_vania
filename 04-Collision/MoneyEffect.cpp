#include "MoneyEffect.h"



MoneyEffect::MoneyEffect(float X, float Y, TAG typeEffectMoney)
{
	_texture = TextureManager::GetInstance()->GetTexture(typeEffectMoney);
	_sprite = new Sprite(_texture, 2000);

	this->x = X + 40;
	this->y = Y - 20;
}

void MoneyEffect::Update(DWORD dt)
{
	Effect::Update(dt);
	if (_sprite->GetCurrentFrame() == 1) // nếu là frame cuối thì xong, frame cuối trống
		isFinish = true;
}

MoneyEffect::~MoneyEffect()
{
}
