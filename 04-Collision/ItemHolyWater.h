#ifndef __ITEMHOLYWATER_H__
#define __ITEMHOLYWATER_H__

#define ITEMHOLYWATER_GRAVITY 0.25f
#define ITEMHOLYWATER_MAXDISPLAYTIME 3000
#define ITEMHOLYWATER_MAXWAITINGTIME 300

#include "Item.h"

class ItemHolyWater : public Item
{
public:
	ItemHolyWater(float X = 0, float Y = 0);
	virtual ~ItemHolyWater();

};


#endif