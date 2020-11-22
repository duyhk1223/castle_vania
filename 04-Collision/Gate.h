#ifndef __GATE_H__
#define __GATE_H__

#include "GameObject.h"

// Các trạng thái của cổng
#define GATE_CLOSE 0

class Gate : public GameObject
{
private:
	int isStart; // Biến xét khi cổng mở

public:
	Gate(float X, float Y);
	virtual ~Gate();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render(Camera* camera);
};

#endif
