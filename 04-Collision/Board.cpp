#include "Board.h"

string Board::FillNumber(string s, UINT MaxNumber)
{
	while (s.size() < MaxNumber)
		s = "0" + s;
	return s;
}

Board::Board(float X, float Y)
{
	_texture = TextureManager::GetInstance()->GetTexture(TAG::BOARD);
	_sprite = new Sprite(_texture, 0);

	_spriteIconDoubleShot = new Sprite(TextureManager::GetInstance()->GetTexture(TAG::ITEMDOUBLESHOT), 0);

	x = X;
	y = Y;

	_boardHealth = new BoardHealth(x + 110, y + 30);
}

void Board::Render(Simon* simon, int stage, int RemainingTime, GameObject* boss)
{
	_sprite->Draw(x, y);

	_font.Draw(x + 100, y + 15, FillNumber(std::to_string(simon->GetScore()), 6)); // Sồ điểm mà người chơi tích luỹ được
	_font.Draw(x + 295, y + 15, FillNumber(std::to_string(RemainingTime), 4)); // Thời gian còn lại để chơi
	_font.Draw(x + 465, y + 15, FillNumber(std::to_string(stage), 2)); // Màn hiện tại mà người chơi đang ở

	_font.Draw(x + 393, y + 33, FillNumber(std::to_string(simon->GetHeartCollect()), 2)); // Số lượng tim người chơi nhặt được
	_font.Draw(x + 393, y + 51, FillNumber(std::to_string(simon->GetLives()), 2)); // Số mạng còn lại của Simon

	int BossBlood = 16;
	if (boss != NULL)
	{
		BossBlood = (int)(boss->GetHealth() * 16 / 24); // HEALTH = 24 -> 16 vạch, nếu level của roi Simon = 1 thì đánh 12 phát chết, 1 phát = 2 máu nên tổng là 24 máu
		if (BossBlood == 0 && boss->GetHealth() > 0)
			BossBlood = 1;
	}

	_boardHealth->Draw(simon->GetHealth(), BossBlood);

	TAG SubWeaponType = simon->GetTypeSubWeapon();
	switch (SubWeaponType)
	{
	case TAG::DAGGER:
		simon->mapWeapon[SubWeaponType]->RenderIcon(x + 310.0f, y + 40.0f);
		break;

	case TAG::HOLYWATER:
		simon->mapWeapon[SubWeaponType]->RenderIcon(x + 312, y + 33);
		break;

	case TAG::THROWINGAXE:
		simon->mapWeapon[SubWeaponType]->RenderIcon(x + 310, y + 35);
		break;

	case TAG::BOOMERANG:
		simon->mapWeapon[SubWeaponType]->RenderIcon(x + 310, y + 35);
		break;

	case TAG::STOPWATCH:
		simon->mapWeapon[SubWeaponType]->RenderIcon(x + 312, y + 31);
		break;
	}

	if (simon->GetIsUsingDoubleShot())
	{
		_spriteIconDoubleShot->Draw(x + 465, y + 35);
	}
}

Board::~Board()
{
	SAFE_DELETE(_sprite);
}

void Board::SetTexure(Texture* texture)
{
	_texture = texture;
	_sprite->texture = texture;
}