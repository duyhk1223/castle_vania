#include "Sprite.h"

RECT Sprite::GetRectFrame(int idFrame)
{
	RECT res;
	res.left = (idFrame % texture->GetColumn()) * texture->GetFrameWidth();
	res.top = (idFrame / texture->GetColumn()) * texture->GetFrameHeight();
	res.right = res.left + texture->GetFrameWidth();
	res.bottom = res.top + texture->GetFrameHeight();
	return res;
}

Sprite::Sprite(Texture* Texture, DWORD TimeAnimation)
{
	texture = Texture;
	currentFrame = 0;
	totalFrames = texture->totalFrames - 1;
	this->timeAnimation = TimeAnimation;
	spriteHandler = Game::GetInstance()->GetSpriteHandler();
}

Sprite::~Sprite()
{

}

void Sprite::MoveToNextFrame()
{
	currentFrame++;
	if (currentFrame > totalFrames) // Điều kiện cho texture có 1 animation
		currentFrame = 0;
}



void Sprite::SelectFrame(int idFrame)
{
	currentFrame = idFrame;
}

void Sprite::Update(DWORD dt)
{
	timeAccumulated += dt;
	if (timeAccumulated >= timeAnimation)
	{
		timeAccumulated -= timeAnimation; // reset time
		this->MoveToNextFrame();
	}
}

void Sprite::Draw(float X, float Y, int alpha, int R, int G, int B)
{
	DrawFrame(currentFrame, X, Y, alpha, R, G, B);
}

void Sprite::DrawFrame(int idFrame, float X, float Y, int alpha, int R, int G, int B)
{
	RECT r = GetRectFrame(idFrame);
	D3DXVECTOR3 p(trunc(X), trunc(Y), 0);
	spriteHandler->Draw(texture->texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, R, G, B));
}

void Sprite::DrawFlipX(float X, float Y, int alpha, int R, int G, int B)
{
	this->DrawFrameFlipX(currentFrame, X, Y, alpha, R, G, B);
}

void Sprite::DrawFrameFlipX(int idFrame, float X, float Y, int alpha, int R, int G, int B)
{
	RECT r = GetRectFrame(idFrame);

	D3DXMATRIX AA, BB;
	spriteHandler->GetTransform(&AA);
	D3DXMatrixTransformation2D(&BB, &D3DXVECTOR2(X, X), 0.0f, &D3DXVECTOR2(-1.0f, 1.0f), NULL, 0.0f, NULL);
	D3DXMATRIX CC = AA * BB;
	spriteHandler->SetTransform(&CC);
	X -= texture->GetFrameWidth();
	D3DXVECTOR3 p(trunc(X), trunc(Y), 0);
	spriteHandler->Draw(texture->texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, R, G, B));
	spriteHandler->SetTransform(&AA); // Reset ve ban dau
}


int Sprite::GetCurrentFrame()
{
	return currentFrame;
}

int Sprite::GetTotalFrames()
{
	return totalFrames;
}