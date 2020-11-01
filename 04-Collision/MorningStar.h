#ifndef __MORNINGSTAR_H__
#define __MORNINGSTAR_H__

#define MORNINGSTAR_TIME_WAIT_ANI 120 // Thời gian chờ chuyển frame của MorningStar

// Animation id cho frame đầu đến cuối của Morning Star Level 1
#define MORNINGSTAR_ANI_LEVEL0_START 0
#define MORNINGSTAR_ANI_LEVEL0_END 3

#include "Weapon.h"
#include "GameObject.h"
#include "TextureManager.h"

class MorningStar : public Weapon
{
protected:
	int level;

public:
	MorningStar();
	~MorningStar();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render(Camera* camera);

	void Attack(float X, float Y, int Direction);

	void UpdatePositionFitSimon(); // Update vị trí cho vũ khí theo vị trí của Simon
	void RenderIcon(float X, float Y); // Hàm render icon lên bảng hiển thị
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void UpgradeLevel();

	int GetLevel();
	//void isCollisionWithAnotherObject(GameObject* object); // Hàm kiểm tra xem vũ khí có va chạm với object khác hay không
};

#endif