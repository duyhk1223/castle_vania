#ifndef __BOARD_H__
#define __BOARD_H__

#include "Sprite.h"
#include "Texture.h"
#include "Camera.h"
#include "define.h"
#include "Font.h"
#include "Simon.h"
#include "BoardHealth.h"
#include "TextureManager.h"
#include "Dagger.h"

#define BOARD_DEFAULT_POSITION_X 0.0f
#define BOARD_DEFAULT_POSITION_Y 0.0f

class Board
{
private:
	Font _font;
	BoardHealth* _boardHealth;

	Texture* _texture;
	Sprite* _sprite;
	float x, y;

	string FillNumber(string s, UINT MaxNumber);

	Sprite* _spriteIconDoubleShot;

public:
	Board(float X, float Y);
	~Board();
	void Render(Simon* simon, int stage /*Màn hiện tại*/, int RemainingTime /*Thời gian chơi còn lại*/, GameObject* boss);

	void SetTexure(Texture* texture);
};

#endif