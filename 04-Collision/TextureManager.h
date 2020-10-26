#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include <unordered_map>
#include "define.h"
#include "GTexture.h"

class TextureManager
{
	unordered_map<objectType, GTexture*> _ArrTextures;

public:
	static TextureManager* __Instance;
	static TextureManager* GetInstance();

	void AddTexture(objectType oType, GTexture* texture);
	GTexture* GetTexture(objectType oType);

	void LoadResource();

	TextureManager();
	~TextureManager();
};

#endif