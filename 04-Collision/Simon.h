#ifndef __SIMON_H__
#define __SIMON_H__


#include "GameObject.h"
#include "Weapon.h"

// Weapon
#include "MorningStar.h"
#include "Item.h"
#include "Dagger.h"
#include "HolyWater.h"
#include "ThrowingAxe.h"
#include "Boomerang.h"
#include "StopWatch.h"


#define SIMON_DEFAULT_POSITION  50.0f, 300.0f

#define SIMON_BBOX_WIDTH 60
#define SIMON_BBOX_HEIGHT 63
#define SIMON_BBOX_SITTING_HEIGHT 45
#define SIMON_BBOX_JUMPING_HEIGHT 45

#define PULL_UP_SIMON_AFTER_SITTING 18.0f // Kéo simon lên 18px sau khi ngồi rồi đứng dậy, tránh overlaping do BBOX bottom thu lại khi ngồi

#define SIMON_VJUMP 0.34f
#define SIMON_VJUMP_HURTING 0.2f // Nhảy lúc bị đau
#define PULL_UP_SIMON_AFTER_JUMPING 18.0f // Kéo simon lên 18px sau khi nhảy, tránh overlaping do BBOX bottom thu lại khi nhảy

#define SIMON_GRAVITY 0.005f 
#define SIMON_GRAVITY_JUMPING 0.001f
#define SIMON_GRAVITY_HURTING 0.001f

#define SIMON_WALKING_SPEED 0.12f //0.12f 

#define SIMON_STATE_IDLE 0
#define SIMON_STATE_WALKING 1


// id frame animation =================================//

#define SIMON_ANI_WALKING_BEGIN 1
#define SIMON_ANI_WALKING_END 3


#define SIMON_ANI_IDLE 0

#define SIMON_ANI_JUMPING 4

#define SIMON_ANI_SITTING 4

#define SIMON_ANI_HURTING 8

// Animaition của phần ngồi đánh
#define SIMON_ANI_SITTING_ATTACKING_BEGIN 15
#define SIMON_ANI_SITTING_ATTACKING_END 17

// Animation cho phần đứng đánh
#define SIMON_ANI_STANDING_ATTACKING_BEGIN 5
#define SIMON_ANI_STANDING_ATTACKING_END 7

// Animation cầu thang
#pragma region Animation trên cầu thang

#define SIMON_SPEED_ONSTAIR 0.09f 

#define SIMON_ANI_STAIR_STANDING_UP 12
#define SIMON_ANI_STAIR_STANDING_DOWN 10

/*Ani đang đi xuống cầu thang đánh*/
#define SIMON_ANI_STAIR_DOWN_ATTACKING_BEGIN 18
#define SIMON_ANI_STAIR_DOWN_ATTACKING_END 20

/*Ani đang đi lên cầu thang đánh*/
#define SIMON_ANI_STAIR_UP_ATTACKING_BEGIN 21
#define SIMON_ANI_STAIR_UP_ATTACKING_END 23

// Id cho frame bắt đầu và kết thúc của frame đi lên và xuống cầu thang
#define SIMON_ANI_STAIR_GO_DOWN_BEGIN 10
#define SIMON_ANI_STAIR_GO_DOWN_END 11

#define SIMON_ANI_STAIR_GO_UP_BEGIN 12
#define SIMON_ANI_STAIR_GO_UP_END 13

#pragma endregion

//====================================================//

#define SIMON_TIME_WAIT_ANI_ATTACKING 120// thời gian thời của mỗi frame khi tấn công

#define SIMON_DEFAULT_HEALTH 16
#define SIMON_DEFAULT_HEARTCOLLECT 5
#define SIMON_DEFAULT_SCORE 0
#define SIMON_DEFAULT_LIVES 3


#define TIME_FREEZE_MAX 500
#define SIMON_UNTOUCHABLE_TIME 2000  // Thời gian Simon ở trạng thái ko thể va chạm khi có va chạm với enemy


class Simon : public GameObject
{
private:

	Camera* camera;

public:
	int HeartCollect; // Số lượng item heart người chơi nhặt được
	int Lives; // số mạng của Simon
	int Score; // Số điểm người chơi kiếm được

	bool isFreeze; // đứng im khi nhặt item nâng cấp
	DWORD TimeFreeze; // thời gian đã đóng băng


#pragma region Các biến back up dữ liệu trc đó cho Simon

	D3DXVECTOR2 PositionBackup;


	bool isWalking_Backup;
	bool isJumping_Backup;
	bool isSitting_Backup;
	bool isAttacking_Backup;
	bool isOnStair_Backup;
	int isProcessingOnStair_Backup;
	int directionStair_Backup;
	int directionY_Backup;

#pragma endregion

	TAG SubWeaponType; // loại vũ khí phụ đang giữ
	bool isUsingDoubleShot; // Biến check xem Simon có sử dụng double shot hay không

	bool isUntouchable;
	DWORD startUntouchableTime; // Thời điểm bắt đầu vào trạng thái ko thể va chạm

	bool isAttacking;
	bool isWalking;
	bool isJumping;
	bool isSitting;
	bool isHurting;

	bool isOnStair;
	int isProcessingOnStair;  // Có 2 giai đoạn lên thang
	int directionStair; // Hướng của cầu thang đang đi, -1 đi qua trái, 1 đi qua phải
	int directionY; // Hướng đi theo trục y của Simon

	int directionAfterGo; // Khoảng cách sau khi đi

	float AutoGoX_Distance; // khoảng cách simon cần tự đi
	float AutoGoX_Speed; // vận tốc đi tự động
	float AutoGoX_GoDirection; // hướng tự động đi
	float AutoGoX_Backup_X; // Lưu vị trí trước khi đi tự động

	float passedDistance = 0; // Độ cao cầu thang Simon đã đi dc

	bool isCollisionAxisYWithBrick = false; // Đang va chạm với đất theo trục y

	bool isAutoGoX = 0; // Biến xét xem Simon có đang ở chế độ auto go không

	unordered_map<TAG, Weapon*> mapWeapon;


public:
	Simon(Camera* camera);
	~Simon();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(Camera* camera);


	void Stop(); // Idle

	void Sit();
	void ResetSit();

	void Go(); // Walk
	void Jump();
	void Right();
	void Left();

	void Attack(TAG weaponType);
	void SetHurt(LPCOLLISIONEVENT e);

	bool GetFreeze();
	void SetFreeze(int f);
	void UpdateFreeze(DWORD dt);

	void StartUntouchable();

	// Các hàm va chạm
	void CollisionWithBrick(const vector<LPGAMEOBJECT>* coObjects = NULL);
	void CollisionWithStair(vector<LPGAMEOBJECT>* coObjects);
	bool isCollisionWithItem(Item* objItem);
	//--------------------------------------------------------------------//


	void SetAutoGoX(int GoDirection, int DirectionAfterGo, float Distance, float WalkSpeed); // set các thông số auto và backup các trạng thái hiện tại
	bool GetIsAutoGoX(); // kiểm tra có đang ở chế độ auto go?

	void RestoreBackupAutoGoX(); // Khôi phục trạng thái 



	void Init(); // khởi tạo lại các trạng thái, HeartCollect, Heath, Lives, Score
	void Reset(); // khởi tạo lại các trạng thái.

	// Các hàm xử lý nhặt cũ khí
	TAG GetTypeSubWeapon();
	void SetTypeSubWeapon(TAG t);
	void PickUpSubWeapon(TAG t);
	bool IsUsingWeapon(TAG weaponType);

	// Điểm và mạng
	int GetLives();
	void SetLives(int lives);
	int GetScore();
	void SetScore(int score);
	int GetHeartCollect();
	void SetHeartCollect(int collectedHeart);

	// Kiểm tra và cho phép dùng double shot
	bool GetIsUsingDoubleShot();
	void SetIsUsingDoubleShot(bool IsUsingDoubleShot);
};



#endif