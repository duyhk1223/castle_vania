#ifndef  __THROWINGAXE_H__
#define __THROWINGAXE_H__

#define THROWINGAXE_GRAVITY 0.0015f
#define THROWINGAXE_SPEED_X 0.2f // Tốc độ bay và rơi
#define THROWINGAXE_SPEED_Y 0.55f

#include "Weapon.h"

class ThrowingAxe : public Weapon
{
private:
	Sprite* _spriteIcon;
	Camera* camera;

public:
	ThrowingAxe(Camera* camera);
	virtual ~ThrowingAxe();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Attack(float X, float Y, int Direction);
	virtual void RenderIcon(float X, float Y);
};

#endif