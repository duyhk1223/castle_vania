#include "Steam.h"

Steam::Steam(float X, float Y, int Model)
{
	_texture = TextureManager::GetInstance()->GetTexture(TAG::STEAM);
	_sprite = new Sprite(_texture, 50);

	dx = dy = vx = vy = 0;
	this->x = X;
	this->y = Y;
	this->_model = Model;

	// Xét dạng bọt nước để set up tốc độ xuất hiện
	switch (_model)
	{

	case STEAM_MODEL_1:
	{
		vy = -0.4f;
		vx = -0.04f;
		break;
	}

	case STEAM_MODEL_2:
	{
		vy = -0.5f;
		vx = 0.0f;
		break;
	}
	case STEAM_MODEL_3:
	{
		vy = -0.4f;
		vx = 0.1f;
		break;
	}

	}
}

Steam::~Steam()
{
}

void Steam::Update(DWORD dt)
{
	// Update quãng đường đi và thời gian
	this->dt = dt;
	this->dx = vx * dt;
	this->dy = vy * dt;

	vy += STEAM_GRAVITY * dt; // Vận tốc rơi

	x += dx;
	y += dy;
}