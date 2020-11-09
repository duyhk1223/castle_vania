#ifndef __MONEYBAG_H__
#define __MONEYBAG_H__

#include "Item.h"

#define MONEYBAG_TIMEDISPLAYMAX 3000
#define MONEYBAG_TIMEWAITMAX 300
#define MONEYBAG_GRAVITY 0.25f

#define MONEYBAG_ANI_RED_BAG 0
#define MONEYBAG_ANI_WHITE_BAG 1
#define MONEYBAG_ANI_PURPLE_BAG 2

class MoneyBag : public Item
{
public:
	MoneyBag(float X, float Y, TAG MoneyBagType);
	virtual ~MoneyBag();

	virtual void Render(Camera* camera);
};

#endif