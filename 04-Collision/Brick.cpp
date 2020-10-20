#include "Brick.h"

Brick::Brick(float X, float Y, int bWidth, int bHeight, int bModel)
{
	brickModel = bModel;

	switch (bModel)
	{
	case BRICK_MODEL_1:
		objectTexture = TextureManager::GetInstance()->GetTexture(objectType::TEX_BRICK_MODEL_1);
		break;
	case BRICK_MODEL_TRANSPARENT:
		objectTexture = TextureManager::GetInstance()->GetTexture(objectType::TEX_BRICK_TRANSPARENT);
		break;
	default:
		DebugOut(L"[BRICK] Get Texture thất bại! Không thể nhận dạng Brick Model!\n");
		break;
	}

	oType = objectType::BRICK;
	objectSprite = new GSprite(objectTexture, 1000);
	this->x = X;
	this->y = Y;
	this->brickWidth = bWidth;
	this->brickHeight = bHeight;
}

void Brick::Render(Camera* camera)
{
	if (IS_DEBUG_RENDER_BBOX)
		RenderBoundingBox(camera);

	if (brickModel == BRICK_MODEL_TRANSPARENT) return;

	D3DXVECTOR2 mapPosition = camera->Transform(x, y);

	// Lấy kích thước gạch chia cho kích thước của map rồi nhân cho kích thước viên gạch để render
	for (int i = 0; i < (int)ceil(brickWidth / objectTexture->GetFrameWidth()); i++)
		for (int j = 0; j < (int)ceil(brickHeight / objectTexture->GetFrameHeight()); j++)
			objectSprite->Draw(mapPosition.x + i * objectTexture->GetFrameWidth(), mapPosition.y + j * objectTexture->GetFrameHeight());
}

void Brick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	// Lấy kích thước 1 viên chia cho chiều dài hoặc chiều cao của nó sau đó nhân cho kích thước 1 viên rồi cộng với toạ độ hiện tại để lấy 
	right = x + (float)ceil(brickWidth / objectTexture->GetFrameWidth()) * objectTexture->GetFrameWidth();
	bottom = y + (float)ceil(brickHeight / objectTexture->GetFrameHeight()) * objectTexture->GetFrameHeight();
}

int Brick::GetBrickModel()
{
	return brickModel;
}