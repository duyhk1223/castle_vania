#ifndef __CROSS_H__
#define __CROSS_H__

#define CROSS_GRAVITY 0.25f
#define CROSS_MAXDISPLAYIME 3000
#define CROSS_MAXWAITINGTIME 300

#define CROSS_COLOR_BACKGROUND D3DCOLOR_XRGB(188, 188, 188) // Màu xám 188, 188, 188, dùng cho effect khi nhặt dc thánh giá

#include "Item.h"

class Cross : public Item
{
public:
	Cross(float X = 0, float Y = 0);
	virtual ~Cross();
};

#endif