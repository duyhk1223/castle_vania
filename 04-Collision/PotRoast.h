#ifndef __POTROAST_H__
#define __POTROAST_H__

#define POTROAST_GRAVITY 0.25f
#define POTROAST_MAXDISPLAYTIME 3000
#define POTROAST_MAXWAITINGTIME 300

#include "Item.h"

class PotRoast : public Item
{
public:
	PotRoast(float X = 0, float Y = 0);
	virtual ~PotRoast();
};

#endif
