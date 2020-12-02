#include "StopWatch.h"

StopWatch::StopWatch()
{
	type = TAG::STOPWATCH;

	texture = TextureManager::GetInstance()->GetTexture(TAG::STOPWATCH);
	sprite = new Sprite(texture, 100);

	isFinish = true; // Ban đầu thì chưa tấn công
}

StopWatch::~StopWatch()
{
}

void StopWatch::Attack(float X, float Y, int Direction)
{
	UsedTime = 0;
	isFinish = false;
	//Sound::GetInstance()->Play(eSound::soundStopWatch, true, 100);
}

void StopWatch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Nếu đã hết thời gian sử dụng thì ko xét
	if (isFinish)
		return;

	UsedTime += dt;
	if (UsedTime >= 4000) // Sử dụng trong 4s
	{
		UsedTime = 0;
		isFinish = true;
	}
}

void StopWatch::Render(Camera* camera)
{
}

bool StopWatch::isCollision(GameObject* object)
{
	return false; // Stop Watch ko xét va chạm
}

void StopWatch::RenderIcon(float X, float Y)
{
	sprite->Draw(X, Y);
}