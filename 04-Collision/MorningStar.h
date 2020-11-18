#ifndef __MORNINGSTAR_H__
#define __MORNINGSTAR_H__

#define MORNINGSTAR_TIME_WAIT_ANI 120 // Thời gian chờ chuyển frame của MorningStar

// Animation id cho frame đầu đến cuối của Morning Star Level 1
#define MORNINGSTAR_ANI_LEVEL0_START 0
#define MORNINGSTAR_ANI_LEVEL0_END 3

#define MORNINGSTAR_ANI_LEVEL1_START 4
#define MORNINGSTAR_ANI_LEVEL1_END 7

#define MORNINGSTAR_ANI_LEVEL2_START 8
#define MORNINGSTAR_ANI_LEVEL2_END 11

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

	void UpdatePositionFitSimon(); 
	void RenderIcon(float X, float Y); // Hàm render icon lên bảng hiển thị
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool isCollision(GameObject* obj); // Kiểm tra va chạm của roi

	void UpgradeLevel();


	int GetLevel();
	
};

#endif