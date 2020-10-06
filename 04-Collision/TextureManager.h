#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <unordered_map>
#include "define.h"
#include "GTexture.h"

class TextureManager
{
	unordered_map<objectType, GTexture*> _allGameTextures;

public:
	static TextureManager* __Instance;
	static TextureManager* GetInstance();

	void AddTextureToMap(objectType type, GTexture* texture);
	GTexture* GetTexture(objectType type);

	void LoadResource();

	TextureManager();
	~TextureManager();
};

#endif