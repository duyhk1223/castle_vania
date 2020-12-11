#include "Helicopter.h"

Helicopter::Helicopter(float X, float Y)
{
	type = TAG::HELICOPTER;
	texture = TextureManager::GetInstance()->GetTexture(TAG::HELICOPTER);
	sprite = new Sprite(texture, 70);

	this->x = X;
	this->y = Y;
	this->direction = -1;
	this->Health = 1;

	vx = -HELICOPTER_SPEED_X;
	vy = HELICOPTER_SPEED_Y *  direction;
}

Helicopter::~Helicopter()
{
}

void Helicopter::Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects)
{
	GameObject::Update(dt); // Update dx, dy, dt

	x += dx;
	y += dy;
}

void Helicopter::Render(Camera* camera)
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