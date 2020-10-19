#include "Texture.h"

int Texture::GetFrameWidth()
{
	return frameWidth;
}

int Texture::GetFrameHeight()
{
	return frameHeight;
}

int Texture::GetColumn()
{
	return column;
}

int Texture::GetRow()
{
	return row;
}

Texture::Texture(char* filePath, int column, int row, int totalframes, int R, int G, int B)
{
	this->column = column;
	this->row = row;
	totalFrames = totalframes;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFileA(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	this->frameWidth = info.Width / column;
	this->frameHeight = info.Height / row;

	LPDIRECT3DDEVICE9 d3ddv = Game::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileExA(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(R, G, B),
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}
}

Texture::~Texture()
{
	if (this->texture != NULL)
		this->texture->Release();
}