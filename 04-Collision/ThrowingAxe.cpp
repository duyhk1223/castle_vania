#include "ThrowingAxe.h"

ThrowingAxe::ThrowingAxe(Camera* camera)
{
	type = TAG::THROWINGAXE;
	texture = TextureManager::GetInstance()->GetTexture(TAG::THROWINGAXE);
	sprite = new Sprite(texture, 70);

	_spriteIcon = new Sprite(TextureManager::GetInstance()->GetTexture(TAG::ITEMTHROWINGAXE), 0);

	isFinish = true;
	this->camera = camera;
}

ThrowingAxe::~ThrowingAxe()
{
}

void ThrowingAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Nếu object nằm ngoài camera thì ko update nữa
	if (!camera->checkObjectInCamera(x, y, (float)camera->GetWidth(), (float)camera->GetHeight()))
	{
		isFinish = true;

		if (GameSound::GetInstance()->GetIsSoundPlaying(Sound::soundAxe))
			GameSound::GetInstance()->Stop(Sound::soundAxe);

		return;
	}

	GameObject::Update(dt); // Update dx, dy, dt
	vy += THROWINGAXE_GRAVITY * dt;

	x += dx;
	y += dy;

	sprite->Update(dt);
}

void ThrowingAxe::Attack(float X, float Y, int Direction)
{
	// Nếu đang tấn công thì ko xét
	if (isFinish == false)
		return;

	Weapon::Attack(X, Y, Direction);
	vx = THROWINGAXE_SPEED_X * direction;
	vy = -THROWINGAXE_SPEED_Y;

	GameSound::GetInstance()->Play(Sound::soundAxe, true);
}

void ThrowingAxe::RenderIcon(float X, float Y)
{
	_spriteIcon->Draw(X, Y);
}