#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <unordered_map>
#include "define.h"
#include "Texture.h"

class TextureManager
{
	unordered_map<TAG, Texture*> _ArrTextures;

public:
	static TextureManager* __Instance;
	static TextureManager* GetInstance();

	void AddTexture(TAG oType, Texture* texture);
	Texture* GetTexture(TAG oType);

	void LoadResource();

	TextureManager();
	~TextureManager();
};

#endif