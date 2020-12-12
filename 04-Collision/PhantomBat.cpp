#include "PhantomBat.h"

PhantomBat::PhantomBat(Simon* simon, Camera* camera, vector <Weapon*>* listWeaponOfEnemy)
{
	type = TAG::PHANTOMBAT;
	texture = TextureManager::GetInstance()->GetTexture(type);
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
	x = PHANTOMBAT_DEFAULT_X;
	y = PHANTOMBAT_DEFAULT_Y;

	Health = PHANTOMBAT_DEFAULT_HEALTH;
	BossStatus = PHANTOMBAT_SLEEP;

	xPreviousPos = x;
	yPreviousPos = y;

	isWaiting = false;
	yLastFrame = y;
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

void PhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetHealth() <= 0)
		return;

	//DebugOut(L"\n Trang thai trc x = %d", BossStatus);



#pragma region Phần Update xử lý các trạng thái của Boss

	switch (BossStatus)
	{
	case PHANTOMBAT_SLEEP:
	{
		break;
	}

	case PHANTOMBAT_HANDLE_START_1: // Trạng thái đi xuống khi tỉnh dậy
	{
		// Bắt đầu thì đi xuống 1 đoạn ngắn (40px)
		if (y >= yTargetPos)
		{
			vy = 0;
			BossStatus = PHANTOMBAT_HANDLE_START_2; // Chuyển boss qua trạng thái di chuyển đến cửa sổ

			xPreviousPos = x; // Backup lại vị trí trc đó của boss
			yPreviousPos = y;

			xTargetPos = 5480; // Vị trí cửa sổ
			yTargetPos = 201;

			vx = ((xTargetPos - xPreviousPos) / (1500.0f)); // Vận tốc cần để đi đến target trong 1.5s
			vy = 0.12f; // Tạo độ cong, để boss di chéo xuống cửa sổ
		}
		break;
	}

	case PHANTOMBAT_HANDLE_START_2: // Đi cong xuống ngay cửa sổ
	{
		if (!isWaiting)
		{
			// Tạo độ cong di chuyển
			vy -= 0.0001f * dt;
			if (vy < 0)
				vy = 0;
			// Tạo độ cong di chuyển

			if (x >= xTargetPos) // Di chuyển xong đến mục tiêu 2 (cửa sổ)
			{
				vx = 0;
				vy = 0;


				isWaiting = true; // Bật trạng thái chờ để dc tấn công
				WaitedTime = 0; // Reset lại time đã chờ để dc tấn công
			}
		}
		else
		{
			WaitedTime += dt;
			if (WaitedTime >= (UINT)(2000 + rand() % 1500)) // Random thời gian cộng thêm trong khoảng 1.5s
			{
				isWaiting = false; // ngừng chờ

				StartCurvesAttack();
			}
		}


		break;
	}

	case PHANTOMBAT_ATTACK_CURVES:
	{
		if (abs(x - xPreviousPos) >= abs(xTargetPos - xPreviousPos)) // Boss đi tới target xong thì đi thẳng
		{
			vx = 0;
			vy = 0;
			isUsingBezierCurves = false;

			StartStaightAttack();

			break;
		}

		// Sử dụng phần trăm đã đi được tương ứng t của Bézier curve (Quãng đường đi dc / tổng quãng đường)
		float perc = (x - xPreviousPos) / (xTargetPos - xPreviousPos);

		float ya = GetPt(y1, y2, perc); // 2 chấm xanh
		float yb = GetPt(y2, y3, perc);

		float yy = GetPt(ya, yb, perc); // Chấm đen

		vy = (yy - yLastFrame /* Khoảng cách y giữa frame trước và y dự tính đi */) / dt;   // v = s / t, khi y = yy = yLastFrame = yTarGetPos thì vy = 0 


		break;
	}

	case PHANTOMBAT_ATTACK_STRAIGHT_1:
	{
		if (abs(x - xPreviousPos) >= abs(xTargetPos - xPreviousPos) ||
			abs(y - yPreviousPos) >= abs(yTargetPos - yPreviousPos)) // Boss đi tới target rồi thì đi thẳng theo hướng random
		{
			vx = vy = 0;

			StartStaightAttack();

		}
		break;
	}

	case PHANTOMBAT_ATTACK_STRAIGHT_2:
	{
		if (!isWaiting)
		{
			if (abs(x - xPreviousPos) >= abs(xTargetPos - xPreviousPos) ||
				abs(y - yPreviousPos) >= abs(yTargetPos - yPreviousPos)) // Nếu boss đi tới target thì dừng
			{
				vx = vy = 0;


				isWaiting = true; // Bật trạng thái chờ để dc tấn công
				WaitedTime = 0; // Reset lại time đã chờ để dc tấn công
			}
		}
		else
		{
			WaitedTime += dt;
			if (WaitedTime >= 3000)
			{
				isWaiting = false; // Ngừng trạng thái chờ

				int random = rand() % 3;
				switch (random)
				{
				case 0: // 33 %
					StartAttackWithWeapon();
					break;

				default: // 66%
					StartCurvesAttack();

					break;
				}
			}
			else
			{
				HandleSmartAttack();
			}
		}


		break;
	}

	case PHANTOMBAT_ATTACK_WITH_WEAPON:
	{
		if (isWaiting)
		{
			WaitedTime += dt;
			if (WaitedTime >= 1500) // Sau 1.5s thì có thế attack
			{
				isWaiting = false; // Ngừng trạng thái chờ
				StartStaightAttack(); // Chuyển Boss qua trạng thái đi thẳng
			}
		}
		break;
	}


	default:
		break;
	}

#pragma endregion



	//	DebugOut(L"simon - boss = %f\n", sqrt((simon->GetX() - x)*(simon->GetX() - x) + (simon->GetY() - y)*(simon->GetY() - y)));


	GameObject::Update(dt);
	x += dx;
	y += dy;


	//DebugOut(L"\n xCam = %f , xcam-r = %f , x = %f , y = %f \n", camera->GetXCam(), camera->GetXCam() + camera->GetWidth(), x, y);



#pragma region Xử lý khi boss out camera

	// Xét nếu Boss ra khỏi cam thì xử lí hướng tiếp theo, (boss ra khỏi camera ở 2 biên màn hình)
	if (x < camera->GetXCam()
		|| camera->GetXCam() + camera->GetWidth() < x + GetWidth()
		|| y < camera->GetYCam()
		|| camera->GetYCam() + camera->GetHeight() < y + GetHeight()
		)
	{

		switch (BossStatus)
		{
		case PHANTOMBAT_ATTACK_CURVES:
		{
			isUsingBezierCurves = false;
			StartStaightAttack();
			break;
		}

		case PHANTOMBAT_ATTACK_STRAIGHT_1:
		{
			StartStaightAttack();
			break;
		}

		case PHANTOMBAT_ATTACK_STRAIGHT_2:
		{
			int random = rand() % 3;
			switch (random)
			{
			case 0: // 33 %
				StartAttackWithWeapon();
				break;

			default: // 66%
				StartCurvesAttack();

				break;
			}

			break;
		}

		}

	}

#pragma endregion

	yLastFrame = y; // Lưu lại y frame hiện tại
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

	D3DXVECTOR2 pos = camera->Transform(x, y);
	sprite->Draw(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
	{
		RenderBoundingBox(camera);

		if (isUsingBezierCurves) // Nếu Boss attack BezierCurves thì mới vẽ, // Xem lý thuyết trong 4 link file.h 
		{
			for (float i = 0; i < 1; i += 0.01f)
			{
				// The Green Line
				float xa = GetPt(x1, x2, i);
				float ya = GetPt(y1, y2, i);
				float xb = GetPt(x2, x3, i);
				float yb = GetPt(y2, y3, i);

				// The Black Dot
				float xx = GetPt(xa, xb, i);
				float yy = GetPt(ya, yb, i);


				RECT rect;
				rect.left = 0;
				rect.top = 0;
				rect.right = 5;
				rect.bottom = 5;
				D3DXVECTOR2 pos = camera->Transform(xx, yy);
				Game::GetInstance()->Draw(
					pos.x,
					pos.y,
					TextureManager::GetInstance()->GetTexture(
						TAG::BBOX)->texture,
					rect.left,
					rect.top,
					rect.right,
					rect.bottom,
					100);

			}
		}

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 15;
		rect.bottom = 15;
		D3DXVECTOR2 pos1 = camera->Transform(xTargetPos, yTargetPos);
		Game::GetInstance()->Draw(
			pos1.x,
			pos1.y,
			TextureManager::GetInstance()->GetTexture(
				TAG::BBOX)->texture,
			rect.left,
			rect.top,
			rect.right,
			rect.bottom,
			100);
	}
}

float PhantomBat::GetPt(float n1, float n2, float perc)
{
	float diff = n2 - n1; // Tính khoảng cách giữa 2 vị trí

	return n1 + (diff * perc); // Tính ra khoảng các mới đã đi dc (diff * percent là % khoảng cách đã đi dc)
}

// Debug
void PhantomBat::Stop()
{
	vx = vy = 0;
}

void PhantomBat::HandleSmartAttack() // xử lí AI
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
	yPreviousPos = y;
	vy = 0.05f;
	vx = 0.0f;
	yTargetPos = y + 40; // Lúc đầu thì đi xuống 40px, đi từ vị trí mặc định lúc đầu
}

void PhantomBat::StartCurvesAttack()
{
	DebugOut(L"Curves!\n");

	// Backup lại vị trí ban đầu
	xPreviousPos = x;
	yPreviousPos = y;

	x1 = x; // (P0), contol point
	y1 = y;

	x2 = simon->GetX(); // Vị trí của Simon (P1), control point
	y2 = simon->GetY() + simon->GetHeight();

	if (simon->GetX() < x) // Nếu Simon bên trái boss
		xTargetPos = camera->GetXCam() - 100 + rand() % ((int)(simon->GetX() - camera->GetXCam() + 100));
	else // Nếu Simon bên phải boss
		xTargetPos = simon->GetX() + simon->GetWidth() + rand() % ((int)(camera->GetXCam() + camera->GetWidth() - (simon->GetX() + simon->GetWidth()) + 100));

	yTargetPos = simon->GetY() + simon->GetHeight() - 100;

	x3 = xTargetPos; // Vị trí cuối đường cong (P2), control point
	y3 = yTargetPos;

	// Vận tốc cần để đi đến target // Quy ước: cứ 1 giây đi 150px, xTargetPos - xPreviousPos là quãng đường đi
	vx = -(x - xTargetPos) / (abs(xTargetPos - xPreviousPos) * 1000.0f / 150);
	vy = 0;

	isUsingBezierCurves = true;
	BossStatus = PHANTOMBAT_ATTACK_CURVES;
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

	xPreviousPos = x; // Backup lại vị trí trc khi tấn công
	yPreviousPos = y;

	// Tấn công 1 đoạn tự biên trái đến 1 vị trí random nằm giữa 2 biên
	xTargetPos = (float)PHANTOMBAT_BOUNDARY_START_STAIGHT_LEFT + rand() % (PHANTOMBAT_BOUNDARY_START_STAIGHT_RIGHT - PHANTOMBAT_BOUNDARY_START_STAIGHT_LEFT);
	yTargetPos = 80.0f + rand() % (190 - 80);

	//	DebugOut(L"StatusProcessing = %d, Target (%f, %f) \n", StatusProcessing, xTarget, yTarget);


	vx = (xTargetPos - xPreviousPos) / (1000); // Vận tốc tấn công trong 1 giây (đến nơi trong 1s), nếu target càng xa thì sẽ bay càng nhanh
	vy = (yTargetPos - yPreviousPos) / (1000);
}

void PhantomBat::StartAttackWithWeapon()
{

	DebugOut(L"ATTACK!\n");
	int DirectionWeapon = 0;
	float xAttackPos = x + GetWidth() / 2; // Vị trí bắn đạn
	float yAttackPos = y + GetHeight() / 2;

	// Xét vị trí Simon để canh hướng đạn
	if (xAttackPos < simon->GetX())
		DirectionWeapon = 1;
	else
		DirectionWeapon = -1;

	// Khoảng cách đạn sẽ bay đến trúng vị trí của Simon
	float S = sqrt((xAttackPos - simon->GetX()) * (xAttackPos - simon->GetX()) + (yAttackPos - simon->GetY()) * (y - simon->GetY())); // S = sqrt(x^2 + y^2)

	// Thời gian bắn trúng nếu dùng vận tốc FIREBALL_SPEED
	float t = S / FIREBALL_SPEED;

	weapon->SetSpeed(DirectionWeapon * abs(xAttackPos - simon->GetX()) / t, abs(yAttackPos - simon->GetY()) / t); // Càng gần thì vận tốc bay càng nhanh
	weapon->Attack(xAttackPos, yAttackPos, 1);

	BossStatus = PHANTOMBAT_ATTACK_WITH_WEAPON;

	//DebugOut(L"vx = %f , vy = %f , isFinish = %d\n", weapon->GetVx(), weapon->GetVy(), weapon->GetFinish());

	//DebugOut(L"!------------------------!\n\n");


	GameSound::GetInstance()->Play(Sound::soundHit);

	WaitedTime = 0;
	isWaiting = true;
}
