#ifndef __BRICK_H__
#define __BRICK_H__

#include "GameObject.h"
#include "TextureManager.h"

#define BRICK_MODEL_1				1 // Gạch của màn 1 loại 32px
#define BRICK_MODEL_TRANSPARENT		4 // Gạch trong suốt

class Brick : public GameObject
{
protected:
	int brickWidth, 
		brickHeight, 
		brickModel;

public:
	Brick(float X, float Y, int bWidth, int bHeight, int bModel);
	virtual void Render(Camera* camera);
	virtual void GetBoundingBox(float &left, float &top, float& right, float &bottom);
	int GetBrickModel();
};

#endif