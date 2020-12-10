#include "GameTime.h"



GameTime::GameTime()
{
	_accumulatedTime = 0;
	SetTime(0);
}

GameTime::~GameTime()
{
}

void GameTime::Update(DWORD dt)
{
	if (_accumulatedTime < 1000) // Nếu thời gian chờ chạy chưa đủ 1s thì sẽ cộng tiếp
	{
		_accumulatedTime += dt;
	}
	else
	{
		_accumulatedTime = (_accumulatedTime + dt) % 1000; // Reset lại thời gian chờ
		_passedTime++;
		isChanged = true; // Set trạng thái thời gian đã có thay đổi vì đã đếm qua dc 1s
	}
}

void GameTime::SetTime(int time)
{
	_passedTime = time;
}

int GameTime::GetPassedTime()
{
	return  _passedTime;
}

bool GameTime::GetIsChanged()
{
	bool b = isChanged;
	isChanged = false; // Sau khi lấy trạng thái xong rồi thì coi như chưa thay đổi, để tránh việc bị replay gọi sound khi thời gian chơi còn <= 30s
	return b;
}