#ifndef __GAMETIME_H__
#define __GAMETIME_H__

#include "define.h"

class GameTime
{
	DWORD _accumulatedTime; // Biến đếm thời gian đã chờ (chờ trong vòng 1s)
	int _passedTime; // Thời gian chơi đã qua
	bool isChanged; // Biến kiểm tra xem biến _gameTime đã được cập nhật hay chưa

public:
	GameTime();
	~GameTime();

	void Update(DWORD dt);
	void SetTime(int time);
	int GetPassedTime(); // Get thời gian đã chơi game
	bool GetIsChanged(); // Hàm check xem thời gian có tay đổi hay chưa để phục vụ cho phần chạy sound khi <= 30s
};

#endif