#include "HiddenObject.h"

HiddenObject::HiddenObject(float X, float Y, int Width, int Height)
{
	this->x = X;
	this->y = Y;
	this->width = Width;
	this->height = Height;
	type = TAG::OBJECT_HIDDEN;
	id = 0; // set sau
	Health = 1;
}

HiddenObject::~HiddenObject()
{
}

void HiddenObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y + height;
}

void HiddenObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void HiddenObject::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
	{
		RenderBoundingBox(camera);
	}
}