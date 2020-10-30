#include "Map.h"

Map::Map()
{

}


Map::~Map()
{
	SAFE_DELETE(_sprite);
}

void Map::LoadMap(TAG mapType)
{
	switch (mapType)
	{
	case TAG::MAP1:
		ReadMapTXT("Resources/map/readfile_map_1.txt");
		break;
	case TAG::MAP2:
		ReadMapTXT("Resources/map/readfile_map_2.txt");
		break;
	default:
		DebugOut(L"[MAP] Load map that bai!");
		return;
		break;
	}

	_texture = TextureManager::GetInstance()->GetTexture(mapType);
	_sprite = new Sprite(_texture, 100);

}

void Map::ReadMapTXT(char* filename)
{
	ifstream inp(filename, ios::in);
	inp >> rowMap >> columnMap >> columnTile >> rowTile >> totalTiles >> heightBoard;

	for (int i = 0; i < rowMap; i++)
		for (int j = 0; j < columnMap; j++)
			inp >> tileMap[i][j];
	inp.close();
}

void Map::DrawMap(Camera* camera)
{
	// Xét xem camera đang nằm ở hàng nào, cột nào để vẽ các tile set tương ứng trong hàng và cột đó
	int row = (int)(camera->GetYCam()) / _texture->GetFrameHeight();
	int column = (int)(camera->GetXCam()) / _texture->GetFrameHeight();

	// Di chuyển map theo camera
	float x = -(float)((int)(camera->GetXCam()) % _texture->GetFrameHeight());
	float y = -(float)((int)(camera->GetYCam()) % _texture->GetFrameHeight());

	/*
	* Load ra số hàng + cột đang nằm trên camera của map (phần đk cộng thêm 1 là vì nếu Simon chưa di chuyển
	  hoàn toàn để cột cuối cùng bên ngoài hiển thị đủ thì nó sẽ ko dc load)
	*/
	for (int i = 0; i < SCREEN_HEIGHT / _texture->GetFrameHeight() + 1; i++)
		for (int j = 0; j < SCREEN_WIDTH / _texture->GetFrameWidth() + 1; j++)
		{
			// Nếu những hàng và cột nằm trong phạm vị camera thì sẽ render
			if (!(row + i < 0 || row + i >= rowMap || j + column < 0 || j + column > columnMap))
				_sprite->DrawFrame(tileMap[row + i][column + j], x + _texture->GetFrameWidth() * j, y + _texture->GetFrameHeight() * i + heightBoard);
		}
}

int Map::GetMapWidth()
{
	return columnMap * _texture->GetFrameWidth();
}

int Map::GetMapHeight()
{
	return rowMap * _texture->GetFrameHeight();
}