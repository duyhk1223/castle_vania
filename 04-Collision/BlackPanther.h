#ifndef __BLACKPANTHER_H__
#define __BLACKPANTHER_H__

#define PANTHER_ANI_SITTING 0 // Ani ngồi của báo
#define PANTHER_ANI_RUNNING_BEGIN 1 // Ani chạy
#define PANTHER_ANI_RUNNING_END 3

#define PANTHER_SPEED_RUNNING 0.18f // Tốc độ chạy và nhảy
#define PANTHER_VYJUMP 0.3f
#define PANTHER_VXJUMP 0.2f

#define PANTHER_GRAVITY 0.005f // Tốc độ rơi và tốc độ nhảy
#define PANTHER_GRAVITY_JUMPING 0.001f

#define PANTHER_DEFAULT_DISTANCE_AUTO_GO_X_DIRECTION_LEFT 177.0f // Khoảng cách Panther tự đi khi chạy bên trái
#define PANTHER_DEFAULT_DISTANCE_AUTO_GO_X_DIRECTION_RIGHT PANTHER_DEFAULT_DISTANCE_AUTO_GO_X_DIRECTION_LEFT - 85 // Khoảng cách Panther tự đi khi chạy bên phải

#include "GameObject.h"
#include "Simon.h"

class BlackPanther : public GameObject
{
	bool isSitting;
	bool isRunning;
	bool isJumping;
	bool isStart; // Trạng thái bắt đầu chạy animation

	float AutoGoX_Distance; // Khoảng cách báo cần tự chạy
	bool isAutoGoX;
	float AutoGoX_Backup;

	Simon* simon;

public:
	BlackPanther(float X, float Y, int Direction, float AutoGoX_Distance, Simon* simon);
	virtual ~BlackPanther();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render(Camera* camera);

	bool GetIsStart();

	void Jump();
	void Run();
};

#endif