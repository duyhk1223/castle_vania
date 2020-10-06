#include "TextureManager.h"

TextureManager* TextureManager::__Instance = NULL;

TextureManager* TextureManager::GetInstance()
{
	if (__Instance == NULL)
		return __Instance = new TextureManager();
	return __Instance;
}

TextureManager::TextureManager()
{
	LoadResource();
}


TextureManager::~TextureManager()
{
	SAFE_DELETE(__Instance);
}

void TextureManager::AddTextureToMap(objectType type, GTexture* texture)
{
	_allGameTextures[type] = texture;
}

GTexture* TextureManager::GetTexture(objectType type)
{
	return _allGameTextures[type];
}

void TextureManager::LoadResource()
{
	// Info Board

	// Player
	AddTextureToMap(objectType::SIMON, new GTexture("Resources/simon.png", 8, 3, 24));
	AddTextureToMap(objectType::SIMON_TRANS, new GTexture("Resources/simon_trans.png", 8, 3, 24));

	/// Scene Intro
	/// 
	/// 
	//----------------------------------------------------------------------------------------------------------------------------------------------//

	/// Map 1
	/// 
	/// 
	// Map
	AddTextureToMap(objectType::MAP1, new GTexture("Resources/map/tileset_map1.png", 8, 4, 32));

	// Game object ground
	AddTextureToMap(objectType::TEX_BRICK_MODEL_1, new GTexture("Resources/ground/2.png")); // Gạch loại dành cho state 1
	AddTextureToMap(objectType::TEX_BRICK_TRANSPARENT, new GTexture("Resources/ground/brick_transparent.png")); // loại trong suốt
	AddTextureToMap(objectType::TORCH, new GTexture("Resources/ground/0.png", 2, 1, 2));

	// Game items
	AddTextureToMap(objectType::ITEMDAGGER, new GTexture("Resources/item/4.png"));
	AddTextureToMap(objectType::LARGEHEART, new GTexture("Resources/item/1.png"));
	AddTextureToMap(objectType::UPGRADEMORNINGSTAR, new GTexture("Resources/item/3.png"));
	AddTextureToMap(objectType::BONUS, new GTexture("Resources/item/2.png", 3, 1, 3));

	// Weapon
	AddTextureToMap(objectType::DAGGER, new GTexture("Resources/weapon/1.png"));
	AddTextureToMap(objectType::MORNINGSTAR, new GTexture("Resources/weapon/morningstar.png", 4, 3, 12));

	// Effect
	AddTextureToMap(objectType::FIRE, new GTexture("Resources/other/1.png", 4, 1, 4));
	AddTextureToMap(objectType::HIT, new GTexture("Resources/other/0.png", 2, 1, 2));
	AddTextureToMap(objectType::EFFECT_MONEY_700, new GTexture("Resources/other/effect700.png", 2, 1, 2));
	AddTextureToMap(objectType::EFFECT_MONEY_100, new GTexture("Resources/other/effect100.png", 2, 1, 2));
	AddTextureToMap(objectType::EFFECT_MONEY_1000, new GTexture("Resources/other/effect1000.png", 2, 1, 2));
	AddTextureToMap(objectType::EFFECT_MONEY_400, new GTexture("Resources/other/effect400.png", 2, 1, 2));
	//----------------------------------------------------------------------------------------------------------------------------------------------//

	/// Map 2
	/// 
	/// 
	//----------------------------------------------------------------------------------------------------------------------------------------------//
}