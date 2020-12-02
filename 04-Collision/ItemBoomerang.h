#ifndef  __ITEMBOOMERANG_H__
#define __ITEMBOOMERANG_H__

#define ITEMBOOMERANG_GRAVITY 0.25f
#define ITEMBOOMERANG_MAXDISPLAYTIME 3000
#define ITEMBOOMERANG_MAXWAITINGTIME 300

#include "Item.h"

class ItemBoomerang :
	public Item
{
public:
	ItemBoomerang(float X = 0, float Y = 0);
	virtual ~ItemBoomerang();
};

#endif