#ifndef __ITEM_H__
#define __ITEM_H__

#include "Sprite.h"
#include "Camera.h"
#include "Texture.h"
#include "GameObject.h"
#include "define.h"

class Item : public GameObject
{
protected:
	DWORD MaxDisplayTime; // Thời gian tối đa để hiển thị item khi rơi ra
	DWORD DisplayedTime; // Thời gian mà item đã được hiển thị

	DWORD MaxWaitingTime; // Thời gian chờ tối đa trước khi item được hiển thị
	DWORD WaitedTime; // Thời gian item đã chờ trước khi được hiển thị

	bool isFinished; // Biến check xem item đã hoàn thành hiển thị hay chưa

public:
	Item();
	virtual ~Item();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects = NULL); // chỉ kiểm tra va chạm với đất
	virtual void Render(Camera* camera);
	virtual bool IsWaitingToDisplay(); // Hàm check nếu đang chờ item hiển thị thì Simon không được nhặt item

	bool GetFinish();
	void SetFinish(bool IsFinished);
};

#endif