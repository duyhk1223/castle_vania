#include "SceneIntroBat.h"

SceneIntroBat::SceneIntroBat(float X, float Y, float VX, float VY)
{
	type = TAG::INTRO_BAT;
	texture = TextureManager::GetInstance()->GetTexture(type);
	sprite = new Sprite(texture, 70);

	this->x = X;
	this->y = Y;
	this->direction = -1;
	this->Health = 1;

	vy = VY;
	vx = VX;
}

SceneIntroBat::~SceneIntroBat()
{
}

void SceneIntroBat::Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects)
{
	GameObject::Update(dt); // Update dx, dy, dt
	x += dx;
	y += dy;

	sprite->Update(dt);
}

void SceneIntroBat::Render(Camera* camera)
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