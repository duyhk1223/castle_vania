#ifndef __ITEMTHROWINGAXE_H__
#define __ITEMTHROWINGAXE_H__

#include "Item.h"
#include "TextureManager.h"

#define ITEMTHROWINGAXE_GRAVITY 0.25f
#define ITEMTHROWINGAXE_MAXDISPLAYTIME 3000
#define ITEMTHROWINGAXE_MAXWAITINGTIME 300

class ItemThrowingAxe : public Item
{
public:
	ItemThrowingAxe(float X = 0, float Y = 0);
	virtual ~ItemThrowingAxe();
};

#endif