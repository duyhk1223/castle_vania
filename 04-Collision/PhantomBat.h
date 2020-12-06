#ifndef __PHANTOMBAT_H__
#define __PHANTOMBAT_H__

/*
https://www.jasondavies.com/animated-bezier/
https://en.wikipedia.org/wiki/B%C3%A9zier_curve
https://stackoverflow.com/questions/785097/how-do-i-implement-a-b%C3%A9zier-curve-in-c
https://www.desmos.com/calculator/lvdgnyhkvy
*/


#define PHANTOMBAT_SLEEP 0

#define PHANTOMBAT_HANDLE_START_1 1 // Lúc bắt đầu, Boss di chuyển xuống
#define PHANTOMBAT_HANDLE_START_2 2 // Lúc bắt đầu, Boss di chuyển đến cửa sổ (nửa vòng cung)

#define PHANTOMBAT_ATTACK_CURVES 3 // Lúc di chuyển theo vòng cung

#define PHANTOMBAT_ATTACK_STRAIGHT_1 4 // Xử lí di chuyển thẳng lần 1
#define PHANTOMBAT_ATTACK_STRAIGHT_2 5 // Xử lí di chuyển thẳng lần 2

#define PHANTOMBAT_ATTACK_WITH_WEAPON 6 // Xử lí tấn công với vũ khí

#define PHANTOMBAT_DEFAULT_X 5295.0f // Vị trí xuất hiện mậc định ban đầu
#define PHANTOMBAT_DEFAULT_Y 90.0f

#define PHANTOMBAT_DEFAULT_HEALTH 24 // BCNN(8,12) = 24 (1 hit sẽ -2 máu), tuỳ theo cấp MorningStar của Simon, cấp 1 và 2 thì 12 hit sẽ chết Bat, còn lại là 8 hit

#define PHANTOMBAT_BOUNDARY_START_STAIGHT_LEFT 5178 // 2 biến giới hạn vùng tấn công của Boss
#define PHANTOMBAT_BOUNDARY_START_STAIGHT_RIGHT 5492


#include "GameObject.h"
#include "Simon.h"
#include "FireBall.h"

class PhantomBat : public GameObject
{
public:
	int BossStatus; // Trạng thái hiện tại của Boss

private:

	float xPreviousPos; // Toạ độ trc đó của Boss, backup lại để phục vụ cho việc tính toán
	float yPreviousPos;

	float xTargetPos; // Toạ độ vị trí Boss sẽ tấn công 
	float yTargetPos;
	Simon* simon;

	/*BezierCurves, hình vòng cung Boss sẽ tấn công*/
	bool isUsingBezierCurves;
	float x1;
	float y1;

	float x2;
	float y2;

	float x3;
	float y3;

	float yLastFrame; // Toạ độ y của frame trước đó
	/*BezierCurves*/

	Camera* camera;


	DWORD WaitedTime; // Thời gian đã chờ trc khi tấn công tiếp
	bool isWaiting; // Trạng thái chờ đen961 khi dc tấn công

	vector <Weapon*>* listWeaponOfEnemy;
	FireBall* weapon;

public:
	PhantomBat(Simon* simon, Camera* camera, vector <Weapon*>* listWeaponOfEnemy);
	~PhantomBat();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(Camera* camera);

	void Start(); // Simon đụng trúng hidden object khởi động boss thì cho boss thức dậy, luồng attack bắt đầu từ hàm này

	// Các hàm xử lý các dạng tấn công
	void StartCurvesAttack();
	void StartStaightAttack();
	void StartAttackWithWeapon();

	// Get toạ độ P theo t (P là 2 chấm xanh)
	float GetPt(float n1, float n2, float perc);
	void Stop(); // Debug

	void HandleAttackType(); // Random ra 1 loại tấn công bất kì dựa vào độ cao nhảy dc của Simon

	void ResetResource();
};

#endif
