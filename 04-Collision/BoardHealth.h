#ifndef __BOARDHEALTH_H__
#define __BOARDHEALTH_H__

#include "Texture.h"
#include "Sprite.h"
#include "define.h"
#include "TextureManager.h"

#define BOARDHEALTH_MAX_CELL 16

#define BOARDHEALTH_FRAME_PLAYER 0
#define BOARDHEALTH_FRAME_NONE 1
#define BOARDHEALTH_FRAME_ENEMY 2

class BoardHealth
{
private:
	Sprite* _sprite;
	Texture* _texture;
	float x, y;

public:
	BoardHealth(float X, float Y);
	~BoardHealth();

	void Draw(int PlayerHealth, int EnemyHealth);
};

#endif