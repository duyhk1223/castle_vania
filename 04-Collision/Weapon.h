#ifndef __WEAPON_H__
#define	__WEAPON_H__

#include "define.h"
#include "Camera.h"
#include "Texture.h"
#include "Sprite.h"
#include "GameObject.h"


class Weapon : public GameObject
{
protected:
	bool isFinish; // Biến check xem animation của vũ khí đã hoàn tất hay chưa
	DWORD LastTimeAttacked; // Lưu lại thời điểm lúc vừa tấn công, làm đánh dấu tránh 1 hit đánh nhiều lần cho các object, có health > 1

public:
	Weapon();
	~Weapon();

	int GetDirection();
	void SetDirection(int Direction);

	virtual void Attack(float X, float Y, int Direction);

	virtual void Render(Camera* camera);
	virtual void UpdatePositionFitSimon(); // Update vị trí cho vũ khí theo vị trí của Simon
	//virtual void isCollisionWithAnotherObject(GameObject* object); // Hàm kiểm tra xem vũ khí có va chạm với object khác hay không
	virtual void RenderIcon(float X, float Y) = 0; // Hàm render icon lên bảng hiển thị
	
	bool GetFinish(); // Get giá trị biến check xem weapon đã hoàn thành attack hay chưa
	void SetFinish(bool IsFinish);

	DWORD GetLastTimeAttacked();
};

#endif