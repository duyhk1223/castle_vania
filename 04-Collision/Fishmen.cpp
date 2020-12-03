#include "Fishmen.h"

Fishmen::Fishmen(float X, float Y, int Direction, Simon* simon, vector<Weapon*>* listWeaponOfEnemy, Camera* camera)
{
	type = TAG::FISHMEN;
	texture = TextureManager::GetInstance()->GetTexture(TAG::FISHMEN);
	sprite = new Sprite(texture, 200);

	this->x = X;
	this->y = Y;
	this->direction = Direction;
	this->simon = simon;
	this->listWeaponOfEnemy = listWeaponOfEnemy;
	this->camera = camera;
	this->Health = 1;

	vx = 0;
	vy = -FISHMEN_SPEED_Y_UP; // Nhảy lên

	xInitPosition = x;
	yInitPosition = y;
	xAccumulatedDistanceToAttack = 0;

	sprite->SelectFrame(FISHMEN_ANI_JUMP); // Mới xuất hiện thì nhảy lên

	isRunning = 0;
	isAttacking = false;
}

Fishmen::~Fishmen()
{
}

void Fishmen::Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects)
{
	// Nếu nhảy lên đủ khoảng cách thì rơi xuống
	if (y <= yInitPosition - FISHMEN_DY_JUMP)
	{
		vy = FISHMEN_SPEED_Y_DOWN;
	}

	if (abs(x - xInitPosition) >= FISHMEN_DX_LIMIT) // Fishmen đã đi đủ khoảng cách quy định
	{
		// Đi về hướng của Simon mà đã vượt Simon thì mới đổi hướng, là hướng hiện tại nếu ngược với hướng của Simon thì đổi hướng
		if ((direction == -1 && !(simon->GetX() < x)) || // Simon đang ở bên trái
			(direction == 1 && !(x < simon->GetX())))	 // Simon đang ở bên phải
		{
			direction *= -1; // Đổi hướng đi
			xInitPosition = x;
		}
	}

	// Nếu đã đi đủ khoảng cách để kích hoạt trạng thái tấn công
	if (xAccumulatedDistanceToAttack >= FISHMEN_DX_ATTACK_LIMIT)
	{
		xAccumulatedDistanceToAttack = 0; // Reset lại biến tính toán
		Attack(); // Tấn công
	}

	xPreviousPosition = x; // Giữ lại vị trí trc đó để phục vụ tính toán khoảng cách di chuyển và kích hoạt tấn công

	// Nếu đang ở trạng thái di chuyển bth
	if (isRunning)
	{
		vx = direction * FISHMEN_SPEED_X;
		vy += FISHMEN_GRAVITY;
	}

	GameObject::Update(dt);


#pragma region Phần xử lí va chạm gạch

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	vector<LPGAMEOBJECT> list_Brick;
	list_Brick.clear();

	for (UINT i = 0; i < listObjects->size(); i++)
		if (listObjects->at(i)->GetType() == TAG::BRICK)
			list_Brick.push_back(listObjects->at(i));

	CalcPotentialCollisions(&list_Brick, coEvents);

	float min_tx, min_ty, nx = 0, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	if (ny == -1)
	{
		vy = 0;
		y += min_ty * dy + ny * 0.4f;
		isRunning = true;
	}
	else
	{
		y += dy;
	}

	if (!isAttacking) // Đang tấn công thì vẫn cho trọng lực kéo xuống
	{
		bool isCollisionDirectionX = false;
		for (UINT i = 0; i < coEventsResult.size(); i++) // Không cho fishmen vượt qua gạch loại nhỏ theo trục x
		{
			if (coEventsResult[i]->nx != 0)
			{
				Brick* brick = dynamic_cast<Brick*>(coEventsResult[i]->obj);
				if (brick->GetModel() == BRICK_MODEL_3)
				{
					x += min_tx * dx + nx * 0.4f;
					direction *= -1; // quay ngược hướng đi 
					isCollisionDirectionX = true;
				}
			}
		}

		if (!isCollisionDirectionX) // ko va chạm với trục x 
			x += dx;
	}


	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

#pragma endregion



	// Nếu đang tấn công thì sẽ tính thời gian chờ để dc di chuyển lại
	if (isAttacking)
	{
		DWORD now = GetTickCount();
		if (now - AttackTime >= FISHMEN_TIME_LIMIT_WAIT_AFTER_ATTACK) // Đã chờ đủ thời gian sau khi attack để di chuyển lại
		{
			isAttacking = false;
		}
	}

	xCurrentPosition = x; // Lưu lại vị trí lúc sau để tính toán cho phần đoạn đường di chuyển tích luỹ để attack
	xAccumulatedDistanceToAttack += abs(xCurrentPosition - xPreviousPosition);




#pragma region Update Fishmen Animation

	// Nếu dang tấn công thì chọn frame attack (frame đầu)
	if (isAttacking)
	{
		sprite->SelectFrame(FISHMEN_ANI_ATTACK);
	}
	else // Di chuyển thì là 2 frame còn lại (1 và 2)
		if (isRunning)
		{
			int index = sprite->GetCurrentFrame();

			if (FISHMEN_ANI_WALK_BEGIN <= index && index <= FISHMEN_ANI_WALK_END)
				sprite->Update(dt);

			if (sprite->GetCurrentFrame() == FISHMEN_ANI_ATTACK) // đang trong trạng thái đi mà quay về frame attack thì set thành frame đi
				sprite->SelectFrame(FISHMEN_ANI_WALK_BEGIN);

		}
#pragma endregion
}

void Fishmen::Render(Camera* camera)
{
	if (Health <= 0)
		return;

	D3DXVECTOR2 pos = camera->Transform(x, y);
	if (direction == -1)
		sprite->Draw(pos.x, pos.y);
	else
		sprite->DrawFlipX(pos.x, pos.y);

	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);
}

void Fishmen::Attack() // Attack gọi trong Update nên weapon sẽ Update theo thời gian
{
	if (isAttacking)
		return;

	// Khởi tạo FireBall mới nếu chưa có hoặc đã kết thúc tấn công
	if (fireBall == NULL)
	{
		fireBall = new FireBall(camera);
		listWeaponOfEnemy->push_back(fireBall);
	}

	// Nếu FireBall còn đang bay
	if (fireBall->GetFinish() == false)
		return;

	isAttacking = true;
	AttackTime = GetTickCount(); // Lưu lại thời điểm tấn công để tính toán thời gian cho phép di chuyển lại

	// Set vị trí tấn công và vận tốc bay cho FireBall
	fireBall->SetSpeed(FIREBALL_SPEED * direction, 0);
	fireBall->Attack(x + 10, y + 3, direction);
}

void Fishmen::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 5;
	top = y + 15;
	right = x + texture->GetFrameWidth() - 5;
	bottom = y + texture->GetFrameHeight();
}