#include "Dagger.h"

Dagger::Dagger(Camera* camera)
{
	type = TAG::DAGGER;
	texture = TextureManager::GetInstance()->GetTexture(TAG::DAGGER);
	sprite = new Sprite(texture, 0);
	this->camera = camera;
}

Dagger::~Dagger()
{
}

void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!camera->checkObjectInCamera(x, y, (float)GetWidth(), (float)GetHeight())) // xử lí ra khỏi cam thì kết thúc 
	{
		isFinish = true;
		return;
	}

	GameObject::Update(dt); // update dt,dx,dy 
	x += dx; // Cập nhật tạo độ khi dao đang bay
}

void Dagger::Attack(float X, float Y, int Direction)
{
	Weapon::Attack(X, Y, Direction);
	vx = DAGGER_SPEED * direction;
}

void Dagger::RenderIcon(float X, float Y)
{
	sprite->DrawFrameFlipX(0, X, Y);
}