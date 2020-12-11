#ifndef __CRYSTALBALL_H__
#define __CRYSTALBALL_H__

#define CRYSTALBALL_GRAVITY 0.25f // Vận tốc rơi
#define CRYSTALBALL_MAXWAITINGTIME 300 // Thời gian hiển thị tối đa
#define CRYSTALBALL_DEFAULT_POSITION_X 5368.0f // Vị trí xuất hiện
#define CRYSTALBALL_DEFAULT_POSITION_y 216.0f

#include "Item.h"

class CrystalBall :
	public Item
{
public:
	CrystalBall(float X, float Y);
	virtual ~CrystalBall();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);
	virtual void Render(Camera* camera);

};

#endif