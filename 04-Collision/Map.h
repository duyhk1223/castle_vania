#ifndef __MAP_H__
#define __MAP_H__

#include "TextureManager.h"
#include "GSprite.h"
#include "define.h"
#include "Camera.h"

class Map
{
private:
	GTexture* mapTexture;
	GSprite* mapSprite;

	int TilesetColumns, TilesetRows, TilesetTotalTiles; // Số hàng, cột, tổng các tile set cần cho map đó

	int MapColumns, MapRows; // Số hàng và cột của map

	int BoardHeight; // Chiều cao của bảng điểm

	int TileMapMatrix[500][500]; // Ma trận chứa các giá trị vị trí tile set

public:
	Map();
	~Map();

	void LoadMapFromResource(objectType mapType);
	void ReadMapFromTXTFile(char *fileName);
	void DrawMap(Camera* camera);
	
	int GetMapWidth();
	int GetMapHeight();
};

#endif