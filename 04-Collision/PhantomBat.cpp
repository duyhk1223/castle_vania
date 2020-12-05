#include "PhantomBat.h"

PhantomBat::PhantomBat(Simon* simon, Camera* camera, vector<Weapon*>* listWeaponOfEnemy)
{
	type = TAG::PHANTOMBAT;
	texture = TextureManager::GetInstance()->GetTexture(TAG::PHANTOMBAT);
	sprite = new Sprite(texture, 70);

	this->simon = simon;
	this->camera = camera;
	this->listWeaponOfEnemy = listWeaponOfEnemy;

	weapon = new FireBall(camera);
	listWeaponOfEnemy->push_back(weapon);

	ResetResource();
}

void PhantomBat::ResetResource()
{
	this->x = PHANTOMBAT_DEFAULT_X;
	this->y = PHANTOMBAT_DEFAULT_Y;

	Health = PHANTOMBAT_DEFAULT_HEALTH;
	BossStatus = PHANTOMBAT_SLEEP;
	
	xPreviousPosition = x; // Backup lại vị trí trc đó của Boss
	yPreviousPosition = y;

	isWaiting = false;
	yLastFramePos = y;

	vx = vy = 0;
	isUsingBezierCurves = false;
}

PhantomBat::~PhantomBat()
{
}

void PhantomBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 5;
	top = y;
	right = x + texture->GetFrameWidth() - 5;
	bottom = y + texture->GetFrameHeight() - 10;
}

float PhantomBat::GetNewDistanceFromPassedDistance(float n1, float n2, float percent)
{
	float diff = n2 - n1;

	return n1 + (diff * percent);
}

void PhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetHealth() <= 0)
		return;

	switch (BossStatus)
	{
	case PHANTOMBAT_SLEEP:
		break;

	case PHANTOMBAT_ATTACK_STRAIGHT_1: // Trạng thái đi xuống khi tỉnh dậy
	{
		// Bắt đầu thì đi xuống 1 đoạn ngắn (40px)
		if (y >= yTargetPosition)
		{
			vy = 0; // Đi đủ thì dừng di chuyển
			BossStatus = PHANTOMBAT_ATTACK_STRAIGHT_2; // Chuyển boss qua trạng thái di chuyển đến cửa sổ

			xPreviousPosition = x; // Backup lại vị trí trc đó của boss
			yPreviousPosition = y;

			xTargetPosition = 5480; // Vị trí cửa sổ
			yTargetPosition = 201;

			vx = ((xTargetPosition - xPreviousPosition) / (1500.0f)); // Vận tốc cần để đi đến target trong 1.5s
			vy = 0.12f; // Tạo độ cong, để boss di chéo dc xuống cửa sổ
		}
		break;
	}

	default:
		break;
	}
}

void PhantomBat::Render(Camera* camera)
{
	if (GetHealth() <= 0)
		return;

	// Xét trạng thái boss để chọn frame tương ứng
	if (BossStatus == PHANTOMBAT_SLEEP)
		sprite->SelectFrame(0);
	else
	{
		sprite->Update(dt);
		if (sprite->GetCurrentFrame() == 0)
			sprite->SelectFrame(1);
	}


}


#pragma region Các hàm xử lý tấn công

// Debug
void PhantomBat::Stop()
{
	vx = vy = 0;
}

void PhantomBat::HandleAttackType()
{
	// Nếu nhảy lên & khoảng cách nhỏ hơn 150 thì random cách xử lí
	if (simon->isJumping && sqrt((simon->GetX() - x) * (simon->GetX() - x) + (simon->GetY() - y) * (simon->GetY() - y)) <= 150.0f)
	{
		int random = rand() % 6;
		switch (random)
		{
		case 0:
			StartStaightAttack();
			return;
		case 1:
			StartCurvesAttack();
			return;
		case 2:
			StartAttackWithWeapon();
			return;

		default:
			break;
		}
	}

	if (rand() % 5 == 0) // 20%
	{
		if (Health <= 10 && simon->isAttacking)
		{
			StartCurvesAttack();
		}
	}
}

void PhantomBat::Start()
{
	BossStatus = PHANTOMBAT_HANDLE_START_1;
	yPreviousPosition = y;
	vy = 0.05f;
	vx = 0.0f;
	yTargetPosition = y + 40; // Lúc đầu thì đi xuống 40px
}

void PhantomBat::StartStaightAttack()
{
	DebugOut(L"Staight!\n");

	switch (BossStatus)
	{
	case PHANTOMBAT_ATTACK_STRAIGHT_1:
		BossStatus = PHANTOMBAT_ATTACK_STRAIGHT_2;
		break;
	default:
		BossStatus = PHANTOMBAT_ATTACK_STRAIGHT_1;
		break;
	}

	xPreviousPosition = x; // Backup lại vị trí trc khi tấn công
	yPreviousPosition = y;

	// Tấn công 1 đoạn tự biên trái đến 1 vị trí random nằm giữa 2 biên
	xTargetPosition = (float)PHANTOMBAT_BOUNDARY_START_STAIGHT_LEFT + rand() % (PHANTOMBAT_BOUNDARY_START_STAIGHT_RIGHT - PHANTOMBAT_BOUNDARY_START_STAIGHT_LEFT);
	yTargetPosition = 80.0f + rand() % (190 - 80); // Độ cao tấn công, >= 80.0f

	//	DebugOut(L"StatusProcessing = %d, Target (%f, %f) \n", StatusProcessing, xTarget, yTarget);


	vx = (xTargetPosition - xPreviousPosition) / (1000); // Vận tốc tấn công trong 1 giây, nếu target càng xa thì sẽ bay càng nhanh
	vy = (yPreviousPosition - yPreviousPosition) / (1000);
}

void PhantomBat::StartCurvesAttack()
{
	DebugOut(L"Curves!\n");

	// Backup lại vị trí ban đầu
	xPreviousPosition = x;
	yPreviousPosition = y;

	x1 = x;
	y1 = y;

	x2 = simon->GetX(); // Vị trí của Simon
	y2 = simon->GetY() + simon->GetHeight();

	if (simon->GetX() < x) // Nếu Simon bên trái boss
		xTargetPosition = camera->GetXCam() - 100 + rand() % ((int)(simon->GetX() - camera->GetXCam() + 100));
	else // Nếu Simon bên phải boss
		xTargetPosition = simon->GetX() + simon->GetWidth() + rand() % ((int)(camera->GetXCam() + camera->GetWidth() - (simon->GetX() + simon->GetWidth()) + 100));

	yTargetPosition = simon->GetY() + simon->GetHeight() - 100;

	x3 = xTargetPosition;
	y3 = yTargetPosition;

	vx = -(x - xTargetPosition) / (abs(xTargetPosition - xPreviousPosition) * 1000.0f / 150); // Vận tốc cần để đi đến target // Quy ước: cứ 1 giây đi 150px
	vy = 0;

	isUsingBezierCurves = true;
	BossStatus = PHANTOMBAT_ATTACK_CURVES;
}

void PhantomBat::StartAttackWithWeapon()
{

	DebugOut(L"ATTACK!\n");
	int WeaponDirection = 0;
	float xAttackPos = x + GetWidth() / 2; // Vị trí bắn đạn
	float yAttackPos = y + GetHeight() / 2;

	// Xét vị trí Simon để canh hướng đạn
	if (xAttackPos < simon->GetX())
		WeaponDirection = 1;
	else
		WeaponDirection = -1;

	// Khoảng cách đạn có thể bắn trúng Simon
	float attackDistance = sqrt((xAttackPos - simon->GetX()) * (xAttackPos - simon->GetX()) + (yAttackPos - simon->GetY()) * (y - simon->GetY())); // attackDistance = sqrt(x^2 + y^2)

	// Thời gian bắn trúng nếu dùng vận tốc FIREBALL_SPEED
	float attackLength = attackDistance / FIREBALL_SPEED;

	weapon->SetSpeed(WeaponDirection * abs(xAttackPos - simon->GetX()) / attackLength, abs(yAttackPos - simon->GetY()) / attackLength); // Càng gần thì vận tốc bay càng nhanh
	weapon->Attack(xAttackPos, yAttackPos, 1);

	BossStatus = PHANTOMBAT_ATTACK;

	//DebugOut(L"vx = %f , vy = %f , isFinish = %d\n", weapon->GetVx(), weapon->GetVy(), weapon->GetFinish());


	//DebugOut(L"!------------------------!\n\n");

	//Sound::GetInstance()->Play(eSound::soundHit);

	WaitedTime = 0;
	isWaiting = true;
}

#pragma endregion