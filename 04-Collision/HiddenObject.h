#pragma once

#include "GameObject.h"

class HiddenObject : public GameObject
{
private:
	int width, height;

public:
	HiddenObject(float X, float Y, int Width, int Height);
	~HiddenObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(Camera* camera);
};