#ifndef __EFFECTMONEY_H__
#define __EFFECTMONEY_H__

#include "Effect.h"
class MoneyEffect :
	public Effect
{
public:
	MoneyEffect(float X, float Y, TAG typeMoneyEffect);
	void Update(DWORD dt);
	virtual ~MoneyEffect();
};

#endif