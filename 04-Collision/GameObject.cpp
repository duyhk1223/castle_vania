﻿

#include "debug.h" 
#include "Game.h"
#include "GameObject.h"

GameObject::GameObject()
{
	x = y = 0;
	vx = vy = 0;
	direction = 1;
	objectHealth = 1;

	LastTimeAttacked = 0;
}

void GameObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + objectTexture->GetFrameWidth();
	bottom = top + objectTexture->GetFrameHeight();
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

int GameObject::GetObjectHealth()
{
	return objectHealth;
}

void GameObject::SetObjectHealth(int h)
{
	objectHealth = h;
}

void GameObject::SubObjectHealth(int th)
{
	objectHealth -= th;
	if (objectHealth < 0)
		objectHealth = 0;
}

void GameObject::SetObjectDirection(int d)
{
	direction = d;
}

int GameObject::GetObjectDirection()
{
	return direction;
}

void GameObject::SetObjectId(int ID)
{
	this->objectId = ID;
}

int GameObject::GetObjectId()
{
	return objectId;
}

void GameObject::SetObjectPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void GameObject::SetObjectSpeed(float vx, float vy)
{
	this->vx = vx;
	this->vy = vy;
}

void GameObject::GetObjectPosition(float& x, float& y)
{
	x = this->x; y = this->y;
}

void GameObject::GetObjectSpeed(float& vx, float& vy)
{
	vx = this->vx;
	vy = this->vy;
}

float GameObject::GetObjectCoordinateX()
{
	return x;
}

float GameObject::GetObjectCoordinateY()
{
	return y;
}

float GameObject::GetObjectVx()
{
	return vx;
}

float GameObject::GetObjectVy()
{
	return vy;
}

void GameObject::SetObjectCoordinateX(float X)
{
	x = X;
}

void GameObject::SetObjectCoordinateY(float Y)
{
	y = Y;
}

void GameObject::SetObjectVx(float VX)
{
	vx = VX;
}

void GameObject::SetObjectVy(float VY)
{
	vy = VY;
}

int GameObject::GetObjectHeight()
{
	return objectTexture->GetFrameHeight();
}

int GameObject::GetObjectWidth()
{
	return objectTexture->GetFrameWidth();
}

objectType GameObject::GetObjectType()
{
	return oType;
}


void GameObject::RenderBoundingBox(Camera* camera)
{
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (LONG)r - (LONG)l;
	rect.bottom = (LONG)b - (LONG)t;

	D3DXVECTOR2 pos = camera->Transform(l, t);

	Game::GetInstance()->Draw(
		pos.x,
		pos.y,
		TextureManager::GetInstance()->GetTexture(objectType::RENDERBBOX)->textureInfo,
		rect.left,
		rect.top,
		rect.right,
		rect.bottom,
		100);
}

LPCOLLISIONEVENT GameObject::SweptAABBEx(GameObject* coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetObjectSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	Game::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CollisionEvent* e = new CollisionEvent(t, nx, ny, coO);
	return e;
}

void GameObject::CalcPotentialCollisions(vector<GameObject*>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);

}

void GameObject::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t <= min_tx && c->nx != 0)
		{
			min_tx = c->t;
			nx = c->nx;
			min_ix = i;
		}

		if (c->t <= min_ty && c->ny != 0)
		{
			min_ty = c->t;
			ny = c->ny;
			min_iy = i;
		}
	}

	if (min_ix >= 0)
		coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0)
		coEventsResult.push_back(coEvents[min_iy]);
}

bool GameObject::isCollitionObjectWithObject(GameObject* obj)	// kiểm tra bằng AABB và Sweept AABB
{
	if (checkAABB(obj)) // kiểm tra va chạm bằng AABB trước
		return true;

	LPCOLLISIONEVENT e = SweptAABBEx(obj); // kt va chạm giữa 2 object bằng sweptAABB
	bool res = e->t > 0 && e->t <= 1.0f; // ĐK va chạm
	SAFE_DELETE(e);
	return res;
}

bool GameObject::checkAABB(GameObject* obj)
{
	float l, t, r, b;
	float l1, t1, r1, b1;
	this->GetBoundingBox(l, t, r, b);
	obj->GetBoundingBox(l1, t1, r1, b1);

	if (Game::GetInstance()->checkAABB(l, t, r, b, l1, t1, r1, b1))
		return true;

	return false;
}

DWORD GameObject::GetLastTimeAttacked()
{
	return LastTimeAttacked;
}

void GameObject::SetLastTimeAttacked(DWORD t)
{
	LastTimeAttacked = t;
}

void GameObject::SetTexture(GTexture* tex)
{
	objectTexture = tex;
	objectSprite->textureInfo = tex;
}

GSprite* GameObject::GetSprite()
{
	return objectSprite;
}

GameObject::~GameObject()
{
	/*SAFE_DELETE(texture);*/
	// ko xóa texture vì đây là texture dùng chung được quản lí bởi TextureManager
	SAFE_DELETE(objectSprite);
}



