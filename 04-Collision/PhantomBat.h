#ifndef __PHANTOMBAT_H__
#define __PHANTOMBAT_H__

#define PHANTOMBAT_SLEEP 0 // Trạng thái ngủ của dơi

#define PHANTOMBAT_HANDLE_START_1 1 // Lúc bắt đầu, Boss di chuyển xuống
#define PHANTOMBAT_HANDLE_START_2 2 // Lúc bắt đầu, Boss di chuyển đến cửa sổ (nửa vòng cung)

#define PHANTOMBAT_ATTACK_CURVES 3 // Lúc di chuyển theo vòng cung

#define PHANTOMBAT_ATTACK_STRAIGHT_1 4 // Xử lí di chuyển tấn công thẳng lần 1
#define PHANTOMBAT_ATTACK_STRAIGHT_2 5 // Xử lí di chuyển tấn công thẳng lần 2

#define PHANTOMBAT_ATTACK 6 // Xử lí tấn công

#define PHANTOMBAT_DEFAULT_X 5295.0f // Vị trí xuất hiện mậc định ban đầu
#define PHANTOMBAT_DEFAULT_Y 90.0f

#define PHANTOMBAT_DEFAULT_HEALTH 24 // BCNN(8,12) = 24 (1 hit sẽ -2 máu), tuỳ theo cấp MorningStar của Simon, cấp 1 thì 12 hit sẽ chết Bat, còn lại là 8 hit

#define PHANTOMBAT_BOUNDARY_START_STAIGHT_LEFT 5178 // 2 biến giới hạn vùng tấn công của Boss
#define PHANTOMBAT_BOUNDARY_START_STAIGHT_RIGHT 5492

#include "GameObject.h"
#include "Simon.h"
#include "FireBall.h"

class PhantomBat : public GameObject
{
private:
	float xPreviousPosition; // Toạ độ trc đó của Boss, backup lại để phục vụ coh việc tính toán
	float yPreviousPosition;

	float xTargetPosition; // Toạ độ vị trí Boss sẽ tấn công 
	float yTargetPosition;
	Simon* simon;

	/*BezierCurves, Hình vòng cung Boss sẽ tấn công*/ 
	bool isUsingBezierCurves;
	float x1;
	float y1;

	float x2;
	float y2;

	float x3;
	float y3;

	float yLastFramePos; // Toạ độ y của frame trước đó
	Camera* camera;

	DWORD WaitedTime; // Thời gian đã chờ trc khi tấn công tiếp
	bool isWaiting;

	// Vũ khí của Boss là FireBall
	vector <Weapon*>* listWeaponOfEnemy;
	FireBall* weapon;

public:
	int BossStatus; // Trạng thái hiện tại của Boss


	PhantomBat(Simon* simon, Camera* camera, vector <Weapon*>* listWeaponOfEnemy);
	~PhantomBat();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(Camera* camera);

	void Start(); // Simon đụng trúng hidden object khởi động boss thì cho boss thức dậy

	// Các hàm xử lý tấn công
	void StartCurvesAttack();
	void StartStaightAttack();
	void StartAttackWithWeapon();

	// Hàm để tính toán và trả về quãng đường đã đi dc trong 1 khoảng cách nào đó dựa vào % x đã đi so với vị trí đến
	float GetNewDistanceFromPassedDistance(float n1, float n2, float perc);
	void Stop(); // Debug

	void HandleAttackType(); // Random ra 1 loại tấn công bất kì dựa vào độ cao nhảy dc của Simon

	void ResetResource();
};

#endif