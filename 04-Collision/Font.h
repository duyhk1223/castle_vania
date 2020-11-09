#ifndef __FONT_H__
#define __FONT_H__

#include "Sprite.h"
#include "Texture.h"
#include "define.h"
#include "TextureManager.h"

class Font
{
private:
	Sprite* _sprite;
	Texture* _texture;

public:
	Font();
	~Font();

	void Draw(float X, float Y, const string& s);
};

#endif