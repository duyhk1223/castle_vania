#include "Grid.h"

Grid::Grid() {}

Grid::~Grid()
{
	// Vòng lặp giải phóng vùng nhớ của Grid
	for (int i = 0; i, GRID_MAP_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_MAP_CELL_MAX_COLUMN; j++)
			mapCells[i][j].clear();
}

void Grid::SetObjectFilePath(char* filePathStr)
{
	objectFilePath = filePathStr;
}

void Grid::ReloadMapGrid()
{
	// Reset lại grid thành rỗng
	for (int i = 0; i < GRID_MAP_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_MAP_CELL_MAX_COLUMN; j++)
			mapCells[i][j].clear();

	int oID, oType, oDirection, oWidth, oHeight, brickModel, numberOfObJects;
	float x, y;

	ifstream fileInfo(objectFilePath, ios::in);
	fileInfo >> numberOfObJects;

	for (int i = 0; i < numberOfObJects; i++)
	{
		fileInfo >> oID >> oType >> oDirection >> x >> y >> oWidth >> oHeight >> brickModel;
		InsertObjectToGrid(oID, oType, oDirection, x, y, oWidth, oHeight, brickModel);
	}

	fileInfo.close();
}

GameObject* Grid::CreateNewGridObject(int oType, float x, float y, int oWidth, int oHeight, int brickModel)
{
	switch (oType)
	{
	case objectType::BRICK:
		 return new Brick(x, y, oWidth, oHeight, brickModel);
		break;
	case objectType::TORCH:
		// return new Torch(x, y);
		break;
	}

	return NULL;
}

void Grid::GetListObjectFromMapGrid(vector<GameObject*>& ObjectList, Camera* camera)
{
	ObjectList.clear();

	unordered_map<int, GameObject*> mapObject;

	int bottom = (int)((camera->GetCamCoordinateY() + camera->GeCamtHeight() - 1) / GRID_CELL_HEIGHT);
	int top = (int)((camera->GetCamCoordinateY() + 1) / GRID_CELL_HEIGHT);

	int left = (int)((camera->GetCamCoordinateX() + 1) / GRID_CELL_WIDTH);
	int right = (int)((camera->GetCamCoordinateX() + camera->GetCamWidth() - 1) / GRID_CELL_WIDTH);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++)
			for (UINT k = 0; k < mapCells[i][j].size(); k++)
			{
				if (mapCells[i][j].at(k)->GetObjectHealth() > 0) // còn tồn tại
				{
					if (mapObject.find(mapCells[i][j].at(k)->GetObjectId()) == mapObject.end()) // ko tìm thấy
						mapObject[mapCells[i][j].at(k)->GetObjectId()] = mapCells[i][j].at(k);
				}
			}

	for (auto& x : mapObject)
	{
		ObjectList.push_back(x.second);
	}
}

void Grid::InsertObjectToGrid(int oID, int oType, int oDirection, float x, float y, int oWidth, int oHeight, int brickModel)
{
	// Lấy vùng chứa các cell đang nằm trong vùng hiển thị
	int top = (int)(y / GRID_CELL_HEIGHT);
	int bottom = (int)((y + oHeight) / GRID_CELL_HEIGHT);
	int left = (int)(x / GRID_CELL_WIDTH);
	int right = (int)((x + oWidth) / GRID_CELL_WIDTH);

	GameObject* obj = CreateNewGridObject(oType, x, y, oWidth, oHeight, brickModel);
	if (obj == NULL)
		return;

	obj->SetObjectId(oID);
	obj->SetObjectDirection(oDirection);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++)
			mapCells[i][j].push_back(obj);
}