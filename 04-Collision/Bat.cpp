#include "Bat.h"


Bat::Bat(float X, float Y, int Direction)
{
	type = TAG::BAT;
	texture = TextureManager::GetInstance()->GetTexture(type);
	sprite = new Sprite(texture, 70);

	this->x = X;
	this->y = Y;
	this->yPositionBackup = y;
	this->direction = Direction;
	this->Health = 1;

	vy = BAT_SPEED_Y;
	vx = BAT_SPEED_X * Direction;
}

Bat::~Bat()
{
}


void Bat::Update(DWORD dt, vector<LPGAMEOBJECT>* listObject)
{
	// Vượt quá biên độ trục y cho phép thì đổi chiều vy
	if (y - yPositionBackup >= DELTA_Y) // Hướng nhấp xuống
	{
		vy = -BAT_SPEED_Y;
	}
	else // Hướng nhấp lên
		if (y - yPositionBackup <= -DELTA_Y)
		{
			vy = BAT_SPEED_Y;
		}


	GameObject::Update(dt); // Update dt, dx, dy

	y += dy;
	x += dx;

	sprite->Update(dt);
	if (sprite->GetCurrentFrame() == 0)
		sprite->Update(dt);
}

void Bat::Render(Camera* camera)
{
	if (Health <= 0)
		return;

	D3DXVECTOR2 pos = camera->Transform(x, y);
	if (direction == -1)
		sprite->Draw(pos.x, pos.y);
	else
		sprite->DrawFlipX(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}
