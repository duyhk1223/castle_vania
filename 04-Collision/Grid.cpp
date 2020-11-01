#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{
	for (int i = 0; i < GRID_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_CELL_MAX_COLUMN; j++)
		{
			cells[i][j].clear();
		}
}

void Grid::SetFilePath(char* str)
{
	filepath = str;
}

void Grid::ReloadGrid() 
{
	// clear grid
	for (int i = 0; i < GRID_CELL_MAX_ROW; i++)
		for (int j = 0; j < GRID_CELL_MAX_COLUMN; j++)
		{
			cells[i][j].clear();
		}

	// đọc file + thêm obj vào grid
	int id, type, direction, w, h, model, n;
	float x, y;

	ifstream inp(filepath, ios::in);
	inp >> n;
	for (int i = 0; i < n; i++)
	{
		inp >> id >> type >> direction >> x >> y >> w >> h >> model;
		Insert(id, type, direction, x, y, w, h, model);
	}
	inp.close();
}

GameObject* Grid::CreateNewObject(int type, float x, float y, int w, int h, int Model)
{
	switch (type)
	{
	case TAG::BRICK:
		return new Brick(x, y, w, h, Model);
	case TAG::TORCH:
		return new Torch(x, y);
	}
	return NULL;
}

void Grid::GetListObject(vector<GameObject*>& ListObj, Camera* camera) // lấy tất cả obj trong camera
{
	ListObj.clear();

	unordered_map<int, GameObject*> mapObject;

	// Tính vị trí các cell đang trong camera
	int bottom = (int)((camera->GetYCam() + camera->GetHeight() - 1) / GRID_CELL_HEIGHT);
	int top = (int)((camera->GetYCam() + 1) / GRID_CELL_HEIGHT);
	int left = (int)((camera->GetXCam() + 1) / GRID_CELL_WIDTH);
	int right = (int)((camera->GetXCam() + camera->GetWidth() - 1) / GRID_CELL_WIDTH);

	// Lấy obj trong từng cell nhét vào mapObject
	for (int i = top; i <= bottom; i++) {
		for (int j = left; j <= right; j++) {
			for (UINT k = 0; k < cells[i][j].size(); k++)
			{
				if (cells[i][j].at(k)->GetHealth() > 0) // còn tồn tại
				{
					if (mapObject.find(cells[i][j].at(k)->GetId()) == mapObject.end()) // không có
						mapObject[cells[i][j].at(k)->GetId()] = cells[i][j].at(k);
				}

			}
		}
	}

	for (auto& x : mapObject)
	{
		ListObj.push_back(x.second);
	}
}

void Grid::Insert(int id, int type, int direction, float x, float y, int w, int h, int Model)
{
	// Tính vị trí của cell từ tọa độ của obj
	int top = (int)(y / GRID_CELL_HEIGHT);
	int bottom = (int)((y + h) / GRID_CELL_HEIGHT);
	int left = (int)(x / GRID_CELL_WIDTH);
	int right = (int)((x + w) / GRID_CELL_WIDTH);

	// Tạo obj
	GameObject* obj = CreateNewObject(type, x, y, w, h, Model);
	if (obj == NULL)
		return;

	obj->SetId(id);
	obj->SetDirection(direction);

	// Nhét vào cell
	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++)
			cells[i][j].push_back(obj);
}