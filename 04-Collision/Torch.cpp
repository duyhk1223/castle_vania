#include "Torch.h"

Torch::Torch(float X, float Y)
{
	texture = TextureManager::GetInstance()->GetTexture(TAG::TORCH);
	sprite = new Sprite(texture, 100);
	this->x = X;
	this->y = Y;
	type = TAG::TORCH;
	Health = 1;
}

Torch::~Torch() 
{
}

void Torch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	sprite->Update(dt);
}

void Torch::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

	D3DXVECTOR2 position = camera->Transform(x, y);
	sprite->Draw(position.x, position.y);
}