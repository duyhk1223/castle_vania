#include "Gate.h"

Gate::Gate(float X, float Y)
{
	type = TAG::GATE;
	texture = TextureManager::GetInstance()->GetTexture(TAG::GATE);
	sprite = new Sprite(texture, 200);
	Health = 1;
	isStart = GATE_CLOSE;
	x = X;
	y = Y;
}

Gate::~Gate()
{
}

void Gate::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + texture->GetFrameWidth() - 40;
	bottom = x + texture->GetFrameHeight();
}

void Gate::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt); // Update dt
}

void Gate::Render(Camera* camera)
{
	switch (isStart)
	{
	case GATE_CLOSE:
		return;
	}

	D3DXVECTOR2 position = camera->Transform(x, y);
	sprite->Draw(position.x, position.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}