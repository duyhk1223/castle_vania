#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Texture.h"

class Sprite
{
private:
	LPD3DXSPRITE spriteHandler;
	int currentFrame;
	int totalFrames;

public:
	Sprite(Texture* Texture, DWORD TimeAnimation);
	virtual ~Sprite();

	Texture* texture;

	RECT GetRectFrame(int idFrame);

	void Draw(float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFrame(int idFrame, float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);

	void DrawFlipX(float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFrameFlipX(int idFrame, float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);

	int GetCurrentFrame();
	int GetTotalFrames();

	void ResetAccumulatedTime();

	//===================== Animation =========================

	DWORD timeAccumulated;            // thời gian từ khi chuyển frame
	DWORD timeAnimation;              // thời gian tối đa của 1 frame


	void MoveToNextFrame();


	void SelectFrame(int idFrame);

	void Update(DWORD dt);
};

#endif