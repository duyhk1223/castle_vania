#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Game.h"
#include "define.h"

class Texture
{
private:
	int frameWidth, frameHeight, column, row;

public:
	int totalFrames;
	LPDIRECT3DTEXTURE9 texture;

	int GetFrameWidth();
	int GetFrameHeight();
	int GetColumn();
	int GetRow();

	Texture(char* filePath, int column = 1, int row = 1, int totalframes = 1, int R = 255, int G = 0, int B = 255);
	~Texture();
};

#endif