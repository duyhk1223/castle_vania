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

void TextureManager::AddTexture(objectType oType, GTexture* texture)
{
	_ArrTextures[oType] = texture;
}

GTexture* TextureManager::GetTexture(objectType oType)
{
	return _ArrTextures[oType];
}

void TextureManager::LoadResource()
{
	// Info Board

	// Player
	AddTexture(objectType::SIMON, new GTexture("Resources/simon.png", 8, 3, 24));
	AddTexture(objectType::SIMON_TRANS, new GTexture("Resources/simon_trans.png", 8, 3, 24));
	AddTexture(objectType::SIMON_DEADTH, new GTexture("Resources/simondeath.png"));

	/// Scene Intro
	/// 
	/// 
	//----------------------------------------------------------------------------------------------------------------------------------------------//

	/// Map 1
	/// 
	/// 
	// Map
	AddTexture(objectType::MAP1, new GTexture("Resources/map/tileset_map1.png", 8, 4, 32));

	// Game object ground
	AddTexture(objectType::TRANSPARENT_BRICK, new GTexture("Resources/ground/brick_transparent.png"));
	AddTexture(objectType::TEX_BRICK_MAP_2, new GTexture("Resources/ground/2-2.png"));
	AddTexture(objectType::TORCH, new GTexture("Resources/ground/0.png", 2, 1, 2));

	// Game items
	AddTexture(objectType::ITEMDAGGER, new GTexture("Resources/item/4.png"));
	AddTexture(objectType::LARGEHEART, new GTexture("Resources/item/1.png"));
	AddTexture(objectType::UPGRADEMORNINGSTAR, new GTexture("Resources/item/3.png"));
	//AddTexture(objectType::BONUS, new GTexture("Resources/item/2.png", 3, 1, 3));

	// Weapon
	AddTexture(objectType::DAGGER, new GTexture("Resources/weapon/1.png"));
	AddTexture(objectType::MORNINGSTAR, new GTexture("Resources/weapon/morningstar.png", 4, 3, 12));

	// Effect
	/*AddTexture(objectType::FIRE, new GTexture("Resources/other/1.png", 4, 1, 4));
	AddTexture(objectType::HIT, new GTexture("Resources/other/0.png", 2, 1, 2));
	AddTexture(objectType::EFFECT_MONEY_700, new GTexture("Resources/other/effect700.png", 2, 1, 2));
	AddTexture(objectType::EFFECT_MONEY_100, new GTexture("Resources/other/effect100.png", 2, 1, 2));
	AddTexture(objectType::EFFECT_MONEY_1000, new GTexture("Resources/other/effect1000.png", 2, 1, 2));
	AddTexture(objectType::EFFECT_MONEY_400, new GTexture("Resources/other/effect400.png", 2, 1, 2));*/
	//----------------------------------------------------------------------------------------------------------------------------------------------//

	/// Map 2
	/// 
	/// 
	//----------------------------------------------------------------------------------------------------------------------------------------------//

	// Others
	AddTexture(objectType::BBOX, new GTexture("Resources/bbox.png"));
}