#ifndef __GRID_H__
#define	__GRID_H__

#include "Game.h"
#include "GSprite.h"
#include "define.h"
#include "GameObject.h"
#include "Brick.h"

using namespace std;

#define GRID_CELL_WIDTH (SCREEN_WIDTH / 4.0f)
#define GRID_CELL_HEIGHT (SCREEN_HEIGHT / 4.0f)

#define GRID_MAP_CELL_MAX_ROW 20 // Số lượng hàng tối đa chứa cell có trong map
#define GRID_MAP_CELL_MAX_COLUMN 100 // Số lượng cột tối đa chứa cell có trong map

class Grid
{
private:
	vector<GameObject*> mapCells[GRID_MAP_CELL_MAX_ROW][GRID_MAP_CELL_MAX_COLUMN]; // Matrix chứa các ô trong map
	char* objectFilePath;

public:
	Grid();
	~Grid();

	void SetObjectFilePath(char* filePathStr);
	void ReloadMapGrid();

	GameObject* CreateNewGridObject(int oType, float x, float y, int oWidth, int oHeight, int brickModel);
	void InsertObjectToGrid(int objectID, int oType, int objectDirection, float x, float y, int oWidth, int oHeight, int brickModel);
	void GetListObjectFromMapGrid(vector<GameObject*>& ObjectList, Camera* camera);
};

#endif
