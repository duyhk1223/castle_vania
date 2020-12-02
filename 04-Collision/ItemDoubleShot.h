#ifndef __ITEMDOUBLESHOT_H__
#define __ITEMDOUBLESHOT_H__

#define ITEMDOUBLESHOT_GRAVITY 0.25f
#define ITEMDOUBLESHOT_MAXDISPLAYTIME 5000
#define ITEMDOUBLESHOT_MAXWAITINGTIME 300

#include "Item.h"

class ItemDoubleShot :
	public Item
{
public:
	ItemDoubleShot(float X = 0, float Y = 0);
	virtual ~ItemDoubleShot();
};

#endif