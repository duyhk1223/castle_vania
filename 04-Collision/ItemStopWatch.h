#ifndef __ITEMSTOPWATCH_H__
#define __ITEMSTOPWATCH_H__

#define STOPWATCH_GRAVITY 0.25f
#define STOPWATCH_MAXDISPLAYTIME 3000
#define STOPWATCH_MAXWAITINGTIME 300


#include "Item.h"

class ItemStopWatch :
	public Item
{
public:
	ItemStopWatch(float X, float Y);
	~ItemStopWatch();
};

#endif