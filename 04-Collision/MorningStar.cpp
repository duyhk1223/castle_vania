#include "MorningStar.h"

MorningStar::MorningStar()
{
	type = TAG::MORNINGSTAR;

	texture = TextureManager::GetInstance()->GetTexture(TAG::MORNINGSTAR);
	sprite = new Sprite(texture, MORNINGSTAR_TIME_WAIT_ANI);
	this->level = 0;
}

MorningStar::~MorningStar()
{
}

void MorningStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameObject::Update(dt);

	// Update để check va chạm

	isFinish = (sprite->GetCurrentFrame() == 3 && level == 0) + (sprite->GetCurrentFrame() == 7 && level == 1) + (sprite->GetCurrentFrame() == 11 && level == 2);

	int StartFrame = MORNINGSTAR_ANI_LEVEL0_START + 4 * level; // ánh xạ chỉ số frame bằng level thay vì dùng if else
	int EndFrame = MORNINGSTAR_ANI_LEVEL0_END + 4 * level;

	if (StartFrame <= sprite->GetCurrentFrame() && sprite->GetCurrentFrame() < EndFrame)
		sprite->Update(dt);
	else
	{
		sprite->SelectFrame(StartFrame);
	}
}

void MorningStar::Render(Camera* camera)
{
	D3DXVECTOR2 position = camera->Transform(x, y);

	if (direction == -1)
		sprite->Draw(position.x, position.y);
	else sprite->DrawFlipX(position.x, position.y);

	if (IS_DEBUG_RENDER_BBOX)
	{
		if (level == 0 && sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL0_START || sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL0_START + 1)
			return; // frame đầu và frame chuẩn bị đánh thì không vẽ BBOX

		if (level == 1 && sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL1_START || sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL1_START + 1)
			return;

		if (level == 2 && sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL2_START || sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL2_START + 1)
			return;
		RenderBoundingBox(camera);
	}
}

void MorningStar::Attack(float X, float Y, int Direction)
{
	Weapon::Attack(X, Y, Direction);

	UpdatePositionFitSimon();

	switch (level)
	{
	case 0:
		sprite->SelectFrame(MORNINGSTAR_ANI_LEVEL0_START);
		break;
	case 1:
		sprite->SelectFrame(MORNINGSTAR_ANI_LEVEL1_START);
		break;
	case 2:
		sprite->SelectFrame(MORNINGSTAR_ANI_LEVEL2_START);
		break;
	}
	sprite->ResetAccumulatedTime();
}

void MorningStar::UpdatePositionFitSimon()
{
	if (direction == -1)
	{
		this->x -= 75;
		this->y -= 2;

	}
	else
	{
		this->x -= 25;
		this->y -= 2;
	}
}

void MorningStar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (level)
	{
	case 0:
		if (direction == 1)
		{
			left = x + 78;
			top = y + 15;
			right = x + texture->GetFrameWidth() - 30;
			bottom = y + texture->GetFrameHeight() - 30;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + texture->GetFrameWidth() - 80;
			bottom = y + texture->GetFrameHeight() - 30;
		}
		break;
	case 1:
		if (direction == 1)
		{
			left = x + 78;
			top = y + 15;
			right = x + texture->GetFrameWidth() - 30;
			bottom = y + texture->GetFrameHeight() - 30;
		}
		else
		{
			left = x + 30;
			top = y + 15;
			right = x + texture->GetFrameWidth() - 80;
			bottom = y + texture->GetFrameHeight() - 30;

		}
		break;
	case 2:
		if (direction == 1)
		{
			left = x + 78;
			top = y + 15;
			right = x + texture->GetFrameWidth();
			bottom = y + texture->GetFrameHeight() - 30;
		}
		else
		{
			left = x;
			top = y + 15;
			right = x + texture->GetFrameWidth() - 80;
			bottom = y + texture->GetFrameHeight() - 30;

		}
		break;
	}
}

bool MorningStar::isCollision(GameObject* obj)
{
	if (level == 0 && sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL0_START || sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL0_START + 1)
		return false; // frame đầu và frame chuẩn bị đánh thì ko xét va chạm

	if (level == 1 && sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL1_START || sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL1_START + 1)
		return false; // frame đầu và frame chuẩn bị đánh thì ko xét va chạm

	if (level == 2 && sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL2_START || sprite->GetCurrentFrame() == MORNINGSTAR_ANI_LEVEL2_START + 1)
		return false; // frame đầu và frame chuẩn bị đánh thì ko xét va chạm

	return Weapon::isCollision(obj);
}

void MorningStar::UpgradeLevel()
{
	if (level >= 2)
		return;

	level++;
	if (isFinish == false) // nếu chưa đánh xong mà update thì phải update lại frame để sau khi Freezed xong sẽ chạy tiếp
	{
		sprite->SelectFrame(sprite->GetCurrentFrame() + 4);
	}
}


void MorningStar::RenderIcon(float X, float Y)
{
}



int MorningStar::GetLevel()
{
	return level;
}

