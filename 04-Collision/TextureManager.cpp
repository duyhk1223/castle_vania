#include "TextureManager.h"

TextureManager* TextureManager::__Instance = NULL;

TextureManager* TextureManager::GetInstance()
{
	if (__Instance == NULL)
		__Instance = new TextureManager();
	return __Instance;
}

void TextureManager::AddTexture(TAG tag, Texture* texture)
{
	_ArrTextures[tag] = texture;
}

Texture* TextureManager::GetTexture(TAG tag)
{
	return _ArrTextures[tag];
}

void TextureManager::LoadResource()
{
	AddTexture(TAG::BBOX, new Texture("Resources/bbox.png"));

	AddTexture(TAG::SIMON,new Texture("Resources/simon.png", 8, 3, 24));
	AddTexture(TAG::SIMON_TRANS, new Texture("Resources/simon_trans.png", 8, 3, 24));
	AddTexture(TAG::SIMON_DEADTH, new Texture("Resources/simondeath.png"));



	// ========== Ground obj ======================
	AddTexture(TAG::TRANSPARENT_BRICK, new Texture("Resources/ground/brick_transparent.png"));
	

	// ========== MAP =============================
	AddTexture(TAG::MAP1, new Texture("Resources/map/tileset_map1.png", 8, 4, 32));

}

TextureManager::TextureManager()
{
	LoadResource();
}


TextureManager::~TextureManager()
{
	SAFE_DELETE(__Instance);
}
