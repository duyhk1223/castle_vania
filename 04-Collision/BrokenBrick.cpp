#include "BrokenBrick.h"

BrokenBrick::BrokenBrick(float X, float Y, int Model)
{
	this->x = X;
	this->y = Y;
	dx = dy = vx = vy = 0;
	_texture = TextureManager::GetInstance()->GetTexture(TAG::BROKENBRICK);
	_sprite = new Sprite(_texture, 3000);
	this->_model = Model;

	// Các hướng tương ứng với các vị trí vỡ gạch
	switch (_model)
	{
	case 1: // Trái, map 2 phần ngoài cùng
	{
		direction = -1;
		vx = direction * 0.15f;
		vy = -0.25f;
		break;
	}

	case 2:// Phải 
	{
		direction = 1;
		vx = direction * 0.15f;
		vy = -0.2f;
		break;
	}

	case 3: // Trái, chỗ lấy miếng gà
	{

		direction = -1;
		vx = direction * 0.07f;
		vy = -0.22f;
		break;	}

	case 4:// Phải
	{

		direction = 1;
		vx = direction * 0.1f;
		vy = -0.3f;
		break;
	}
	}
}

// Update rơi gạch vỡ
void BrokenBrick::Update(DWORD dt)
{
	this->dt = dt;
	this->dx = vx * dt;
	this->dy = vy * dt;

	vy += BROKENBRICK_GRAVITY * dt;

	x += dx;
	y += dy;

	Effect::Update(dt);

	if (_sprite->GetCurrentFrame() == _sprite->GetTotalFrames()) // Nếu là frame cuối thì xong effect
		isFinish = true;
}

BrokenBrick::~BrokenBrick()
{
}