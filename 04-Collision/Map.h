#ifndef __MAP_H__
#define __MAP_H__

#include "TextureManager.h"
#include "Sprite.h"
#include "define.h"
#include "Camera.h"

class Map
{
private:
	Texture* _texture;
	Sprite* _sprite;

	int columnTile, rowTile, totalTiles;

	int columnMap, rowMap;

	int heightBoard; // chiều cao board

	int tileMap[500][500];

public:
	Map();
	~Map();
	void LoadMap(TAG mapType);
	void ReadMapTXT(char* filename);
	void DrawMap(Camera* camera);

	int GetMapWidth();
	int GetMapHeight();
};

#endif