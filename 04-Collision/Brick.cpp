#include "Brick.h"




Brick::Brick(float X, float Y, int W, int H, int Model)
{
	model = Model;
	switch (Model)
	{
	case BRICK_MODEL_2:
		texture = TextureManager::GetInstance()->GetTexture(TAG::TEX_BRICK_MAP_2); // Loại màn 2
		break;
	case BRICK_MODEL_TRANSPARENT:
		texture = TextureManager::GetInstance()->GetTexture(TAG::TRANSPARENT_BRICK);// Loại trong suốt 
		break;
	case BRICK_MODEL_3_3_32:
		texture = TextureManager::GetInstance()->GetTexture(TAG::TEX_BRICK_3_32);// Loại 3 ô nhỏ - 32px 
		break;
	case BRICK_MODEL_3_4_32:
		texture = TextureManager::GetInstance()->GetTexture(TAG::TEX_BRICK_4_32);// Loại đủ 4 ô nhỏ - 32px
		break;
	default:
		DebugOut(L"[BRICK] Get Texture that bai! Ko nhan dang duoc Model!\n");
		break;
	}

	type = TAG::BRICK;
	sprite = new Sprite(texture, 1000);
	this->x = X;
	this->y = Y;
	this->width = W;
	this->height = H;
}

void Brick::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

	if (model == BRICK_MODEL_TRANSPARENT)
		return;

	D3DXVECTOR2 pos = camera->Transform(x, y);

	// Lấy kích thước gạch chia cho kích thước của map rồi nhân cho kích thước viên gạch để render
	for (int i = 0; i < (int)ceil(width / texture->GetFrameWidth()); i++)
		for (int j = 0; j < (int)ceil(height / texture->GetFrameHeight()); j++)
			sprite->Draw(pos.x + i * texture->GetFrameWidth(), pos.y + j * texture->GetFrameHeight());
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + (float)ceil(width / texture->GetFrameWidth()) * texture->GetFrameWidth();
	b = y + (float)ceil(height / texture->GetFrameHeight()) * texture->GetFrameHeight();
}

int Brick::GetModel()
{
	return model;
}
