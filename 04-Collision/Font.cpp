#include "Font.h"

Font::Font()
{
	_texture = TextureManager::GetInstance()->GetTexture(TAG::FONT);
	_sprite = new Sprite(_texture, 0);
}

Font::~Font()
{
	SAFE_DELETE(_sprite);
}

void Font::Draw(float X, float Y, const string& s)
{
	for (UINT i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			_sprite->SelectFrame(s[i] - 'A');
		}
		else
			if (s[i] >= '0' && s[i] <= '9')
			{
				_sprite->SelectFrame(s[i] - '0' + 26);
			}
			else
				_sprite->SelectFrame(36);
		_sprite->Draw(X + i * _texture->GetFrameWidth(), Y);
	}
}