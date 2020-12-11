#ifndef __HELICOPTER_H__
#define __HELICOPTER_H__

#define HELICOPTER_SPEED_X 0.02f // Vận tốc di chuyển theo trục x
#define HELICOPTER_SPEED_Y 0.008f // Vận tốc di chuyển theo trục y

#include "GameObject.h"

class Helicopter : public GameObject
{
public:
	Helicopter(float X = 0, float Y = 0);
	~Helicopter();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* listOjects = NULL);
	void Render(Camera* camera);
};

#endif
