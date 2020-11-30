#ifndef __SMALLHEART_H__
#define __SMALLHEART_H__

#include "Item.h"

#define SMALLHEART_GRAVITY 0.04f
#define SMALLHEART_MAXDISPLAYTIME 6000
#define SMALLHEART_MAXWAITINGTIME 150

#define SMALLHEART_SPEED_X 0.05f

#define DeltaX 25 // px, biên độ dao động của trái tim

class SmallHeart : public Item
{
private:
	float xBackup; // Vị trí của x ban dầu

public:
	SmallHeart(float X = 0, float Y = 0);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects = NULL); // Update rơi và xét va chạm đất

	virtual ~SmallHeart();
};

#endif