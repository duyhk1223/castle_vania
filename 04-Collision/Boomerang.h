#ifndef  __BOOMERANG_H__
#define __BOOMERANG_H__


#define BOOMERANG_SPEED_X 0.3f
#define BOOMERANG_STATUS_DIRECTION_1 1 // Hướng 1 ném theo hướng simon
#define BOOMERANG_STATUS_DIRECTION_2 2 // Hướng 2 ném ngược hướng ban đầu
#define BOOMERANG_LIMIT_DISTANCE_X 285.0f // Boomerang đi được 285px thì đổi hướng

#include "Weapon.h" 
#include "GameSound.h"

class Boomerang :
	public Weapon
{
private:
	Camera* camera;
	Sprite* _spriteIcon;
	int DirectionStatus;

	float initXPosition; // Vị trí x boomerang lúc khởi tạo

	GameObject* simon;

public:
	Boomerang(Camera* camera, GameObject* simon);

	virtual void RenderIcon(float X, float Y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Attack(float X, float Y, int Direction);

	virtual ~Boomerang();
};

#endif