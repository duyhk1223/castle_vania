#ifndef  __HOLLYWATER_H__
#define __HOLLYWATER_H__

#define HOLLYWATER_SPEED_X 0.22f // Vấn tốc bay và rơi của Holy Water
#define HOLLYWATER_SPEED_Y 0.18f
#define HOLYWATER_GRAVITY 0.001f

#include "Weapon.h"

class HolyWater : public Weapon
{
private:
	Sprite* _spriteIcon;
	bool isCollisionWithBrick; // Nếu rớt xuống đất thì sẽ nổ
	int ExplosionEffectCountLoop; // Số lần lặp lại việc vẽ lúc nổ Holywater
	Camera* camera;

public:
	HolyWater(Camera* camera);
	virtual ~HolyWater();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void UpdatePositionFitSimon();
	void Attack(float X, float Y, int Direction);
	void RenderIcon(float X, float Y);
	void Render(Camera* camera);
};

#endif