#ifndef __GRID_H__
#define	__GRID_H__

#include "Game.h"
#include "Sprite.h"
#include "define.h"
#include "GameObject.h"
#include "Brick.h"
#include "Torch.h"
#include "HiddenObject.h"
#include "Candle.h"
#include "StairTop.h"
#include "StairBottom.h"
#include "Gate.h"

using namespace std;

#define GRID_CELL_WIDTH (SCREEN_WIDTH / 4.0f)
#define GRID_CELL_HEIGHT (SCREEN_HEIGHT / 4.0f)

#define GRID_CELL_MAX_ROW 10 // số dòng tối đa của grid
#define GRID_CELL_MAX_COLUMN 50 // số cột tối đa của grid


class Grid
{
private:
	vector<GameObject*> cells[GRID_CELL_MAX_ROW][GRID_CELL_MAX_COLUMN];
	char* filepath;

public:
	Grid();
	~Grid();

	void SetFilePath(char* str); // Set đường dẫn file Object
	void ReloadGrid();


	GameObject* CreateNewObject(int type, float x, float y, int w, int h, int Model);
	void Insert(int id, int type, int direction, float x, float y, int w, int h, int Model); //Thêm object vào grid
	void GetListObject(vector<GameObject*>& ListObj, Camera* camera);
};

#endif
