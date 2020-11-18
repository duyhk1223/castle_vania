#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Sprite.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Camera.h"



class Effect
{
protected:
	Texture* _texture;
	Sprite* _sprite;

	float x;
	float y;
	bool isFinish;

public:
	Effect();
	virtual ~Effect();

	virtual void Render(Camera* camera);
	virtual void Update(DWORD dt);

	bool GetFinish();
	void SetFinish(int b);
};



#endif 