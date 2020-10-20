#include "Map.h"

Map::Map() {}

Map::~Map() 
{
	SAFE_DELETE(mapSprite);
}

void Map::LoadMapFromResource(objectType mapType)
{
	switch (mapType)
	{
	case objectType::MAP1:
		ReadMapFromTXTFile("Resources/map/readfile_map_1.txt");
		break;
	case objectType::MAP2:
		ReadMapFromTXTFile("Resources/map/readfile_map_2.txt");
		break;
	default:
		DebugOut(L"[MAP] Load map thất bại!");
		break;
	}

	mapTexture = TextureManager::GetInstance()->GetTexture(mapType);
	mapSprite = new GSprite(mapTexture, 100);
};

void Map::ReadMapFromTXTFile(char* fileName)
{
	ifstream mapInfo(fileName, ios::in);

	mapInfo >> MapRows >> MapColumns >> TilesetColumns >> TilesetRows >> TilesetTotalTiles >> BoardHeight;
	for (int i = 0; i < MapRows; i++)
	{
		for (int j = 0; j < MapColumns; j++)
		{
			
			mapInfo >> TileMapMatrix[i][j];

		}
	}

	mapInfo.close();
}

void Map::DrawMap(Camera* camera)
{
	// Xét xem camera đang nằm ở hàng nào, cột nào để vẽ các tile set tương ứng trong hàng và cột đó
	int row = (int)(camera->GetCamCoordinateY()) / mapTexture->GetFrameHeight();
	int column = (int)(camera->GetCamCoordinateX()) / mapTexture->GetFrameHeight();

	// Di chuyển map theo camera
	float x = -(float)((int)(camera->GetCamCoordinateX()) % mapTexture->GetFrameHeight());
	float y = -(float)((int)(camera->GetCamCoordinateY()) % mapTexture->GetFrameHeight());

	/*
	* Load ra số hàng + cột đang nằm trên camera của map (phần đk cộng thêm 1 là vì nếu Simon chưa di chuyển 
	  hoàn toàn để cột cuối cùng bên ngoài hiển thị đủ thì nó sẽ ko dc load)
	*/
	for (int i = 0; i < SCREEN_HEIGHT / mapTexture->GetFrameHeight() + 1; i++)
		for (int j = 0; j < SCREEN_WIDTH / mapTexture->GetFrameWidth() + 1; j++)
		{
			// Nếu những hàng và cột nằm ngoài phạm vị camera thì sẽ ko render
			if (!(row + i < 0 || row + i >= MapRows || j + column < 0 || j + column > MapColumns))
				mapSprite->DrawFrame(TileMapMatrix[row + i][column + j], x + mapTexture->GetFrameWidth() * j, y + mapTexture->GetFrameHeight() * i + BoardHeight);
		}
}

int Map::GetMapHeight()
{
	return MapRows * mapTexture->GetFrameHeight();
}

int Map::GetMapWidth()
{
	return MapColumns * mapTexture->GetFrameWidth();
}