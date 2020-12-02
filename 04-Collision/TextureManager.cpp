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

void TextureManager::AddTexture(TAG oType, Texture* texture)
{
	_ArrTextures[oType] = texture;
}

Texture* TextureManager::GetTexture(TAG oType)
{
	return _ArrTextures[oType];
}

void TextureManager::LoadResource()
{
	// Player
	AddTexture(TAG::SIMON, new Texture("Resources/simon.png", 8, 3, 24));
	AddTexture(TAG::SIMON_TRANS, new Texture("Resources/simon_trans.png", 8, 3, 24));
	AddTexture(TAG::SIMON_DEADTH, new Texture("Resources/simondeath.png"));

	// Bounding Box
	AddTexture(TAG::BBOX, new Texture("Resources/bbox.png"));
	

	// Map
	AddTexture(TAG::MAP1, new Texture("Resources/map/tileset_map1.png", 8, 4, 32));
	AddTexture(TAG::MAP2, new Texture("Resources/map/tileset_map2.png", 22, 4, 88));

	// Game object ground
	AddTexture(TAG::TRANSPARENT_BRICK, new Texture("Resources/ground/brick_transparent.png"));
	AddTexture(TAG::TEX_BRICK_MAP_2, new Texture("Resources/ground/2-2.png"));
	AddTexture(TAG::TEX_BRICK_3_32, new Texture("Resources/ground/8-2.png")); // loại nhỏ 4 ô
	AddTexture(TAG::TEX_BRICK_4_32, new Texture("Resources/ground/8-1.png")); // loại nhỏ 4 ô

	AddTexture(TAG::TORCH, new Texture("Resources/ground/0.png", 2, 1, 2));
	AddTexture(TAG::CANDLE, new Texture("Resources/ground/1.png", 2, 1, 2));

	AddTexture(TAG::GATE, new Texture("Resources/ground/Gate1.png", 4, 1, 4));

	// Game items
	AddTexture(TAG::ITEMDAGGER, new Texture("Resources/item/4.png"));
	AddTexture(TAG::LARGEHEART, new Texture("Resources/item/1.png"));
	AddTexture(TAG::UPGRADEMORNINGSTAR, new Texture("Resources/item/3.png"));
	AddTexture(TAG::BONUS, new Texture("Resources/item/2.png", 3, 1, 3));
	AddTexture(TAG::SMALLHEART, new Texture("Resources/item/0.png"));
	AddTexture(TAG::POTROAST, new Texture("Resources/item/10.png"));
	AddTexture(TAG::ITEMDOUBLESHOT, new Texture("Resources/item/11.png"));
	AddTexture(TAG::ITEMHOLYWATER, new Texture("Resources/item/9.png"));
	AddTexture(TAG::ITEMTHROWINGAXE, new Texture("Resources/item/7.png"));
	AddTexture(TAG::INVISIBILITYPOTION, new Texture("Resources/item/14.png"));
	AddTexture(TAG::CROSS, new Texture("Resources/item/6.png"));
	AddTexture(TAG::ITEMBOOMERANG, new Texture("Resources/item/8.png"));

	// Effect
	AddTexture(TAG::HIT, new Texture("Resources/other/0.png", 2, 1, 2));
	AddTexture(TAG::FIRE, new Texture("Resources/other/1.png", 4, 1, 4));
	AddTexture(TAG::BROKENBRICK, new Texture("Resources/other/BrokenBrick.png", 2, 1, 2));

	AddTexture(TAG::EFFECT_MONEY_700, new Texture("Resources/other/effect700.png", 2, 1, 2));
	AddTexture(TAG::EFFECT_MONEY_100, new Texture("Resources/other/effect100.png", 2, 1, 2));
	AddTexture(TAG::EFFECT_MONEY_1000, new Texture("Resources/other/effect1000.png", 2, 1, 2));
	AddTexture(TAG::EFFECT_MONEY_400, new Texture("Resources/other/effect400.png", 2, 1, 2));

	// Weapon
	AddTexture(TAG::DAGGER, new Texture("Resources/weapon/1.png"));
	AddTexture(TAG::MORNINGSTAR, new Texture("Resources/weapon/morningstar.png", 4, 3, 12));
	AddTexture(TAG::HOLYWATER, new Texture("Resources/weapon/2.png", 4, 1, 4));
	AddTexture(TAG::THROWINGAXE, new Texture("Resources/weapon/3.png", 4, 1, 4));
	AddTexture(TAG::BOOMERANG, new Texture("Resources/weapon/4.png", 3, 1, 3));
	AddTexture(TAG::STOPWATCH, new Texture("Resources/item/5.png"));

	// Board
	AddTexture(TAG::FONT, new Texture("Resources/font/1.png", 13, 3, 37));
	AddTexture(TAG::BOARD, new Texture("Resources/blackboard.png", 1, 1, 1));
	AddTexture(TAG::BOARDHEALTH, new Texture("Resources/heal.png", 3, 1, 3));	
	AddTexture(TAG::BOARD_TRANS, new Texture("Resources/blackboard.png", 1, 1, 1, 0, 0, 0));

	// Enemy
	AddTexture(TAG::GHOST, new Texture("Resources/enemy/1.png", 2, 1, 2));
	AddTexture(TAG::PANTHER, new  Texture("Resources/enemy/2.png", 4, 1, 4));
	AddTexture(TAG::BAT, new Texture("Resources/enemy/0.png", 4, 1, 4));
}