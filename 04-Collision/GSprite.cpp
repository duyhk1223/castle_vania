#include "GSprite.h"

int GSprite::GetObjectAnimationCurrentFrame()
{
	return objectCurrentAnimationFrame;
}

int GSprite::GetTextureTotalFrames()
{
	return textureTotalFrames;
}

RECT GSprite::GetFrameRect(int frameID)
{
	RECT frameRect;
	frameRect.left = (frameID % textureInfo->GetTextureColumn()) * textureInfo->GetFrameWidth();
	frameRect.top = (frameID / textureInfo->GetTextureRow()) * textureInfo->GetFrameHeight();
	frameRect.right = frameRect.left + textureInfo->GetFrameWidth();
	frameRect.bottom = frameRect.top + textureInfo->GetFrameHeight();

	return frameRect;
}

GSprite::GSprite(GTexture* texture, DWORD frameAnimationTime)
{
	this->textureInfo = texture;
	objectCurrentAnimationFrame = 0;
	textureTotalFrames = texture->textureTotalFrames - 1; // frame đếm từ 0
	this->frameAnimationTime = frameAnimationTime;
	spriteHandler = Game::GetInstance()->GetSpriteHandler();
}

GSprite::~GSprite()
{

}

void GSprite::moveToNextFrame()
{
	objectCurrentAnimationFrame++;
	if (objectCurrentAnimationFrame > textureTotalFrames)
		objectCurrentAnimationFrame = 0;
}

void GSprite::ReseAccumulatedtTime()
{
	animationFrameAccumulatedTime = 0;
}

void GSprite::SelectObjectAnimationFrame(int frameID)
{
	objectCurrentAnimationFrame = frameID;
}

void GSprite::Update(DWORD dt)
{
	animationFrameAccumulatedTime += dt;
	if (animationFrameAccumulatedTime >= frameAnimationTime)
	{
		animationFrameAccumulatedTime -= frameAnimationTime;
		this->moveToNextFrame();
	}
}

void GSprite::DrawFrame(int idFrame, float X, float Y, int alpha, int R, int G, int B)
{
	RECT r = GetFrameRect(idFrame);
	D3DXVECTOR3 p(trunc(X), trunc(Y), 0);
	spriteHandler->Draw(this->textureInfo->textureInfo, &r, NULL, &p, D3DCOLOR_ARGB(alpha, R, G, B));
}

// Vẽ frame nghịch đảo
void GSprite::Draw(float X, float Y, int alpha, int R, int G, int B)
{
	DrawFrame(objectCurrentAnimationFrame, X, Y, alpha, R, G, B);
}

void GSprite::DrawFrameFlipX(int idFrame, float X, float Y, int alpha, int R, int G, int B)
{
	RECT r = GetFrameRect(idFrame);

	D3DXMATRIX AA, BB;
	spriteHandler->GetTransform(&AA);
	D3DXMatrixTransformation2D(&BB, &D3DXVECTOR2(X, X), 0.0f, &D3DXVECTOR2(-1.0f, 1.0f), NULL, 0.0f, NULL);
	D3DXMATRIX CC = AA * BB;
	spriteHandler->SetTransform(&CC);
	X -= textureInfo->GetFrameWidth();
	D3DXVECTOR3 p(trunc(X), trunc(Y), 0);
	spriteHandler->Draw(textureInfo->textureInfo, &r, NULL, &p, D3DCOLOR_ARGB(alpha, R, G, B));
	spriteHandler->SetTransform(&AA); // Reset về hướng ban đầu
}

void GSprite::DrawFlipX(float X, float Y, int alpha, int R, int G, int B)
{
	this->DrawFrameFlipX(objectCurrentAnimationFrame, X, Y, alpha, R, G, B);
}