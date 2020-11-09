﻿#include "BoardHealth.h"

BoardHealth::BoardHealth(float X, float Y)
{
	_texture = TextureManager::GetInstance()->GetTexture(TAG::BOARDHEALTH);
	_sprite = new Sprite(_texture, 0);
	this->x = X;
	this->y = Y;
}

BoardHealth::~BoardHealth()
{
	SAFE_DELETE(_sprite);
}

void BoardHealth::Draw(int PlayerHealth, int EnemyHealth)
{
	PlayerHealth = min(PlayerHealth, BOARDHEALTH_MAX_CELL);
	EnemyHealth = min(EnemyHealth, BOARDHEALTH_MAX_CELL);

	// Vẽ máu của enemy với Simon
	int i, j;
	// Vã máu còn lại của Simon
	_sprite->SelectFrame(BOARDHEALTH_FRAME_PLAYER);
	for (i = 0; i < PlayerHealth; i++)
		_sprite->Draw(x + i * _texture->GetFrameWidth(), y);

	// Vẽ máu đã mất của Simon
	_sprite->SelectFrame(BOARDHEALTH_FRAME_NONE);
	for (j = i; j < BOARDHEALTH_MAX_CELL; j++)
	{
		_sprite->Draw(x + j * _texture->GetFrameWidth(), y);
	}

	// Vẽ máu còn lại của Enemy
	_sprite->SelectFrame(BOARDHEALTH_FRAME_ENEMY);
	for (i = 0; i < EnemyHealth; i++)
		_sprite->Draw(x + i * _texture->GetFrameWidth(), y + 18);

	// Vẽ máu đã mất của Enemy
	_sprite->SelectFrame(BOARDHEALTH_FRAME_NONE);
	for (j = i; j < BOARDHEALTH_MAX_CELL; j++)
	{
		_sprite->Draw(x + j * _texture->GetFrameWidth(), y + 18);
	}
}