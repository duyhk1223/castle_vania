#include "Camera.h"

Camera::Camera(int width, int height)
{
	_camWidth = width;
	_camHeight = height;
	IsFollowingSimon = true;
	IsSimonAutoGoX = false;

	// Biên mặc định ban đầu là kích thước của map
	_boundaryLeft = 0;
	_boundaryRight = 0;
	_xCamBackup = _yCamBackup = 0;
	SetBoundaryBackUp(_boundaryLeft, _boundaryRight); // Lưu trữ toạ độ mặc định ban đầu
	vx = 0;
}

Camera::~Camera() {}

void Camera::Update(DWORD dt)
{
	this->dt = dt;

	// Nếu Simon đang tự đi
	if (IsSimonAutoGoX)
	{
		// Cập nhật đoạn đường và vị trí của Cam
		float dx = vx * dt;
		_camCoordinateX += dx;

		if (abs(_camCoordinateX - AutoGoX_Backup_X) >= AutoGoX_Distance)
		{
			_camCoordinateX -= (abs(_camCoordinateX - AutoGoX_Backup_X) - AutoGoX_Distance);
			IsSimonAutoGoX = false;
		}
	}

	// Nếu đi tới 2 biên
	if (_camCoordinateX < _boundaryLeft)
		_camCoordinateX = _boundaryLeft;

	if (_camCoordinateX > _boundaryRight)
		_camCoordinateX = _boundaryRight;
}

D3DXVECTOR2 Camera::Transform(float xWorld, float yWorld)
{
	return D3DXVECTOR2(xWorld - _camCoordinateX, yWorld - _camCoordinateY);
}

void Camera::SetCameraPosition(float x, float y)
{
	_camCoordinateX = x;
	_camCoordinateY = y;
}

float Camera::GetCamCoordinateX()
{
	return _camCoordinateX;
}

float Camera::GetCamCoordinateY()
{
	return _camCoordinateY;
}

int Camera::GetCamWidth()
{
	return _camWidth;
}

int Camera::GeCamtHeight()
{
	return _camHeight;
}

bool Camera::CheckIsObjectInCamera(float x, float y, int width, int height) // Toạ độ và kích thước của object
{
	// Nếu object nằm ngài theo chiều ngang
	if (x + width < _camCoordinateX || _camCoordinateX + _camWidth < x)
		return false;
	// Nếu object nằm ngoài theo chiều dọc
	if (y + height < _camCoordinateY || _camCoordinateY + _camHeight < y)
		return false;
	return true;
}

bool Camera::GetIsFollowingSimon()
{
	return IsFollowingSimon;
}

void Camera::SetIsFollowingSimon(bool IsFollowingSimon)
{
	this->IsFollowingSimon = IsFollowingSimon;
}

void Camera::SetAutoGoX(float Distance, float Speed)
{
	if (IsSimonAutoGoX)
		return;
	vx = Speed;
	AutoGoX_Backup_X = _camCoordinateX;
	AutoGoX_Distance = Distance;
	IsSimonAutoGoX = true;
	IsFollowingSimon = false;
}

void Camera::StopAutoGoX()
{
	IsSimonAutoGoX = false;
}

bool Camera::GetIsAutoGoX()
{
	return IsSimonAutoGoX;
}

void Camera::SetCameraBoundary(float boundaryLeft, float boundaryRight)
{
	_boundaryLeft = boundaryLeft;
	_boundaryRight = boundaryRight;
}

float Camera::GetBoundaryLeft()
{
	return _boundaryLeft;
}

float Camera::GetBoundaryRight()
{
	return _boundaryRight;
}

// Back up lại các giá trị vị trí của Camera
void Camera::SetPositionBackUp(float X, float Y)
{
	_xCamBackup = X;
	_yCamBackup = Y;
}

// Back up lại các giá trị biên
void Camera::SetBoundaryBackUp(float boundaryBackUpLeft, float boundaryBackUpRight)
{
	_boundaryLeftBackup = boundaryBackUpLeft;
	_boundaryRightBackup = boundaryBackUpRight;
}

// Reset lại các giá trị theo giá trị dc backup trc đó
void Camera::RestoreBoundary()
{
	_camCoordinateX = _xCamBackup;
	_camCoordinateY = _yCamBackup;

	_boundaryLeft = _boundaryLeftBackup;
	_boundaryRight = _boundaryRightBackup;
}

