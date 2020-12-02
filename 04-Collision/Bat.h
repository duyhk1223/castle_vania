#ifndef __BAT_H__
#define __BAT_H__

#include "GameObject.h" 

#define BAT_SPEED_X 0.1f // Hướng bay theo trục x và y
#define BAT_SPEED_Y 0.05f

#define DELTA_Y 20 // Biên giao động trục y, dùng để tính toán để đổi chiều nhấp lên hoặc xuống của dơi


class Bat : public GameObject
{
private:
	float yPositionBackup; // Vị trí y ban dầu

public:
	Bat(float X = 0, float Y = 0, int Direction = -1); // Thường ban đầu dơi sẽ bay từ phải qua trái
	virtual ~Bat();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);
	void Render(Camera* camera);

};

#endif