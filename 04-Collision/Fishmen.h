#ifndef __FISHMEN_H__
#define __FISHMEN_H__

#define FISHMEN_SPEED_Y_UP 0.4f // Tốc độ nhảy lên khỏi hồ nước và tốc độ rơi xuống
#define FISHMEN_SPEED_Y_DOWN 0.3f

#define FISHMEN_DY_JUMP 350 // Nhảy lên khoảng 330px thì rớt xuống

#define FISHMEN_ANI_JUMP 2 // Ani lúc nhảy
#define FISHMEN_ANI_ATTACK 0 // Ani lúc fishmen attack

#define FISHMEN_ANI_WALK_BEGIN 1 // Ani bắt đầu di chuyển
#define FISHMEN_ANI_WALK_END 2 

#define FISHMEN_GRAVITY 0.01f // Tốc độ rơi xuống khi ko va chạm gạch
#define FISHMEN_SPEED_X 0.07f // Tốc di chuyển theo trục x

#define FISHMEN_DX_LIMIT 140 // Đi được khoảng 140px thì đổi hướng
#define FISHMEN_DX_ATTACK_LIMIT 130 // Đi được khoảng 140px thì đổi hướng

#define FISHMEN_TIME_LIMIT_WAIT_AFTER_ATTACK 700 // Thời gian fishmen phải đứng lại chờ trước khi đi tiếp, sau khi attack

#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "FireBall.h"

class Fishmen : public GameObject
{
	float yInitPosition; // Vị trí x và y lúc khởi tạo
	float xInitPosition;

	float xPreviousPosition; // 2 biến lưu khoảng cách trc và sau để tính toán cho phần khoảng cách di chuyển để attack
	float xCurrentPosition;
	float xAccumulatedDistanceToAttack; // Quãng đường đã đi tích lũy, để khi đi đủ giới hạn sẽ attack
	DWORD AttackTime; // Thời điểm attack

	bool isRunning;
	FireBall* fireBall;
	bool isAttacking;

	/* Lưu thành phần ngoài phục vụ xử lí */
	Simon* simon;
	vector<Weapon*>* listWeaponOfEnemy;
	Camera* camera;

public:
	Fishmen(float X, float Y, int Direction, Simon* simon, vector<Weapon*>* listWeaponOfEnemy, Camera* camera);
	virtual ~Fishmen();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);
	void Render(Camera* camera);
	void Attack();
};

#endif