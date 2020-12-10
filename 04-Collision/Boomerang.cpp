#include "Boomerang.h"


Boomerang::Boomerang(Camera* camera, GameObject* simon)
{
	type = TAG::BOOMERANG;
	texture = TextureManager::GetInstance()->GetTexture(type);
	sprite = new Sprite(texture, 70);

	_spriteIcon = new Sprite(TextureManager::GetInstance()->GetTexture(TAG::ITEMBOOMERANG), 0);

	isFinish = true; // Ban đầu chưa tấn công
	this->camera = camera;
	this->simon = simon;
}


void Boomerang::RenderIcon(float X, float Y)
{
	_spriteIcon->Draw(X, Y);

}

void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{


	GameObject::Update(dt); // Update dx, dy, dt
	x += dx; // Tiếp tục di chuyển theo hướng tấn công

	switch (DirectionStatus)
	{
	case BOOMERANG_STATUS_DIRECTION_1: // Hướng 1: boomerang đi theo hướng ném của Simon
	{
		// Khoảng cách giữa vị trí bắt đầu và hiện tại vượt quá 285 thì đổi hướng, hoặc nằm ngoài camera thì đổi hướng
		if (abs(initXPosition - x) >= BOOMERANG_LIMIT_DISTANCE_X || !camera->CHECK_OBJECT_IN_CAMERA(this))
		{
			x -= dx;
			vx *= -1;
			DirectionStatus = BOOMERANG_STATUS_DIRECTION_2;
		}
		break;
	}

	case BOOMERANG_STATUS_DIRECTION_2: // Hướng 2: boomerang đi ngược hướng ném ban đầu
	{
		if (!camera->CHECK_OBJECT_IN_CAMERA(this) || isCollision(simon)) // Nếu boomerang ra khỏi cam, hoặc quay lại chạm simon thì dừng tấn công
		{
			isFinish = true;
			if (GameSound::GetInstance()->GetIsSoundPlaying(Sound::soundBoomerang)) // Ngừng âm thanh sử dụng boomerang
				GameSound::GetInstance()->Stop(Sound::soundBoomerang);
		}
		break;
	}

	}

	sprite->Update(dt);
}

void Boomerang::Attack(float X, float Y, int Direction)
{
	if (isFinish == false)
		return;
	Weapon::Attack(X, Y, Direction);

	vx = BOOMERANG_SPEED_X * Direction;
	vy = 0;

	DirectionStatus = BOOMERANG_STATUS_DIRECTION_1; // Ném theo hướng của Simon

	initXPosition = X;

	GameSound::GetInstance()->Play(Sound::soundBoomerang, true);
}

Boomerang::~Boomerang()
{
}
