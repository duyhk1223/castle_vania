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

	DWORD timeAccumulated;	 // thời gian chờ đã tích lũy
	DWORD timeAnimation; // thời gian phải chờ giữa các frame

	RECT GetRectFrame(int idFrame);
	void MoveToNextFrame();
	void ResetTime();

	void SelectFrame(int idFrame);

	void Update(DWORD dt);

	void Draw(float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFrame(int idFrame, float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);

	void DrawFlipX(float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFrameFlipX(int idFrame, float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);

	int GetCurrentFrame();
	int GetTotalFrames();
};

#endif