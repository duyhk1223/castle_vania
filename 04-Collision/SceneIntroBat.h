#ifndef __INTROBAT_H__
#define __INTROBAT_H__

#include "GameObject.h"

class SceneIntroBat : public GameObject
{
public:
	SceneIntroBat(float X = 0, float Y = 0, float VX = 0, float VY = 0);
	~SceneIntroBat();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* listObjects = NULL);
	void Render(Camera* camera);
};

#endif