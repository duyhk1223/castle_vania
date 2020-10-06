#ifndef __GSPRITE_H__
#define __GSPRITE_H__

#include "GTexture.h"

class GSprite
{
private:
	LPD3DXSPRITE spriteHandler;
	int objectCurrentAnimationFrame;
	int textureTotalFrames;

public:
	GSprite(GTexture* Texture, DWORD frameAnimationTime);
	virtual ~GSprite();

	GTexture* textureInfo;

	DWORD animationFrameAccumulatedTime;	 // thời gian chờ đã tích lũy (Thời gian tích luỹ để 1 frame trong animation hoàn thành)
	DWORD frameAnimationTime; // thời gian chạy animation của 1 frame (thời gian frame tồn tại trc khi chuyển qua frame mới)

	RECT GetFrameRect(int frameID);
	void moveToNextFrame();
	void ReseAccumulatedtTime();

	void SelectObjectAnimationFrame(int idFrame); // Hàm để dời đến 1 frame khác của animation

	void Update(DWORD dt);

	void Draw(float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFrame(int idFrame, float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);

	// Vẽ frame nghịch đảo
	void DrawFlipX(float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFrameFlipX(int idFrame, float X, float Y, int alpha = 255, int R = 255, int G = 255, int B = 255);

	int GetObjectAnimationCurrentFrame();
	int GetTextureTotalFrames();
};

#endif