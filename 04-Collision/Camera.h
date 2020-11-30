#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "define.h"

class Camera
{
protected:
protected:
	// Toạ độ cũa camera
	float _xCam;
	float _yCam;

	// Toạ độ backup cho camera
	float _xCamBackup;
	float _yCamBackup;
	float _boundaryLeftBackup;
	float _boundaryRightBackup;

	// kích thước camera
	int _width;
	int _height;

	float _boundaryLeft; // Biên giới hạn bên trái 
	float _boundaryRight; // Biên giới hạn bên phải, không bao gồm đoạn simon đi được ở cuối - SCREEN_WIDTH 

	float vx; // Vận tốc di chuyển camera
	DWORD dt;

	bool isAllowFollowSimon;

	bool isAutoGoX; // Biến check camera có đang ở chế độ tự đi không
	float AutoGoX_Distance; // Khoảng cách cần tự động di chuyển
	float AutoGoX_Backup_X; // Vị trí camera trước khi vào chế độ tự đi

public:
	Camera(int w, int h);
	~Camera();

	void Update(DWORD dt);

	D3DXVECTOR2 Transform(float, float);

	void SetPosition(float x, float y);

	float GetXCam();
	float GetYCam();
	int GetWidth();
	int GetHeight();

	bool checkObjectInCamera(float x, float y, float w, float h);

	bool AllowFollowSimon();
	void SetAllowFollowSimon(bool b);

	void SetBoundary(float left, float right); // set biên giới hạn cho camera
	float GetBoundaryRight();
	float GetBoundaryLeft();

	void SetAutoGoX(float Distance, float Speed); // Set các thông số khi trong trạng thái tự di chuyển
	void StopAutoGoX();
	bool GetIsAutoGoX();

	void SetBoundaryBackup(float l, float r);
};

#endif