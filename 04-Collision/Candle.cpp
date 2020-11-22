#include "Candle.h"

Candle::Candle(float X, float Y)
{
	texture = TextureManager::GetInstance()->GetTexture(TAG::CANDLE);
	sprite = new Sprite(texture, 100);
	this->x = X;
	this->y = Y;
	type = TAG::CANDLE;
	Health = 1;
}

Candle::~Candle()
{
}

void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + texture->GetFrameWidth() + 5;
	bottom = y + texture->GetFrameHeight() + 5; // Bounding Box rộng hơn để dễ đánh trúng hơn
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	sprite->Update(dt);
}

void Candle::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

	D3DXVECTOR2 position = camera->Transform(x, y);
	sprite->Draw(position.x, position.y);
}