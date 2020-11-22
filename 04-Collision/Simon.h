#ifndef __SIMON_H__
#define __SIMON_H__


#include "GameObject.h"
#include "Weapon.h"
#include "MorningStar.h"
#include "Item.h"
#include "Dagger.h"

#define SIMON_DEFAULT_POSITION  50.0f, 300.0f

#define SIMON_BBOX_WIDTH 60
#define SIMON_BBOX_HEIGHT 63
#define SIMON_BBOX_SITTING_HEIGHT 45
#define SIMON_BBOX_JUMPING_HEIGHT 45

#define PULL_UP_SIMON_AFTER_SITTING 18.0f // Kéo simon lên 18px sau khi ngồi rồi đứng dậy, tránh overlaping do BBOX bottom thu lại khi ngồi

#define SIMON_VJUMP 0.34f
#define SIMON_VJUMP_HURTING 0.2f // nhảy lúc bị đau
#define PULL_UP_SIMON_AFTER_JUMPING 18.0f // Kéo simon lên 18px sau khi nhảy, tránh overlaping do BBOX bottom thu lại khi nhảy

#define SIMON_GRAVITY 0.005f 
#define SIMON_GRAVITY_JUMPING 0.001f

#define SIMON_WALKING_SPEED 0.12f //0.12f 

#define SIMON_STATE_IDLE 0
#define SIMON_STATE_WALKING 1


// id frame animation =================================//

#define SIMON_ANI_WALKING_BEGIN 1
#define SIMON_ANI_WALKING_END 3


#define SIMON_ANI_IDLE 0

#define SIMON_ANI_JUMPING 4

#define SIMON_ANI_SITTING 4

// Animaition của phần ngồi đánh
#define SIMON_ANI_SITTING_ATTACKING_BEGIN 15
#define SIMON_ANI_SITTING_ATTACKING_END 17

// Animation cho phần đứng đánh
#define SIMON_ANI_STANDING_ATTACKING_BEGIN 5
#define SIMON_ANI_STANDING_ATTACKING_END 7

//====================================================//

#define SIMON_TIME_WAIT_ANI_ATTACKING 120// thời gian thời của mỗi frame khi tấn công

#define SIMON_DEFAULT_HEALTH 16
#define SIMON_DEFAULT_HEARTCOLLECT 5
#define SIMON_DEFAULT_SCORE 0
#define SIMON_DEFAULT_LIVES 3


#define TIME_FREEZE_MAX 500


class Simon : public GameObject
{
private:

	Camera* camera;
	TAG SubWeaponType; // loại vũ khí phụ đang giữ

	int HeartCollect; // Số lượng item heart người chơi nhặt được
	int Lives; // số mạng của Simon
	int Score; // Số điểm người chơi kiếm được

	bool isFreeze; // đứng im khi nhặt item nâng cấp
	DWORD TimeFreeze; // thời gian đã đóng băng

	bool isCollisionAxisYWithBrick = false; // Đang va chạm với đất theo trục y

public:
	
	bool isAttacking;
	bool isWalking;
	bool isJumping;
	bool isSitting;

	

	unordered_map<TAG, Weapon*> mapWeapon;


	Simon(Camera* camera);
	~Simon();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(Camera* camera);


	void Stop(); // idle

	void Sit();
	void ResetSit();

	void Go(); // walk
	void Jump();
	void Right();
	void Left();

	void Attack(TAG weaponType);

	bool GetFreeze();
	void SetFreeze(int f);
	void UpdateFreeze(DWORD dt);

	void CollisionWithBrick(const vector<LPGAMEOBJECT>* coObjects = NULL);
	bool isCollisionWithItem(Item* objItem);


	TAG GetTypeSubWeapon();
	void SetTypeSubWeapon(TAG t);
	void PickUpSubWeapon(TAG t);


	void Init(); // khởi tạo lại các trạng thái, HeartCollect, Heath, Lives, Score
	void Reset(); // khởi tạo lại các trạng thái.

	// Điểm và mạng
	int GetLives();
	void SetLives(int lives);
	int GetScore();
	void SetScore(int score);
	int GetHeartCollect();
	void SetHeartCollect(int collectedHeart);
};



#endif