#ifndef __UPGRADEMORNINGSTAR_H__
#define __UPGRADEMORNINGSTAR_H__

#include "Item.h"
#include "TextureManager.h"

#define UPGRADEMORNINGSTAR_GRAVITY 0.25f
#define UPGRADEMORNINGSTAR_TIMEDISPLAYMAX 3000
#define UPGRADEMORNINGSTAR_TIMEWAITMAX 300

class UpgradeMorningStar : public Item
{
	UpgradeMorningStar(float X = 0, float Y = 0);
	~UpgradeMorningStar();
};

#endif