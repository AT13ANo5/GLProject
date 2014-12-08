#include "Camera.h"
#include "Mouse.h"
#include <math.h>
float ClickPosX;

CCamera* CCamera::CurrentCamera = nullptr;

CCamera::CCamera()
{
	_Eye.x = 
	_Eye.y = 
	_Eye.z = 0;
	
	_Lookat.x =
	_Lookat.y = 0;
	_Lookat.z = -50.0f;

	_UpVec.x =
	_UpVec.z = 0;
	_UpVec.y = 1.0f;

	CurrentCamera = this;
}

CCamera::~CCamera()
{
	CurrentCamera = nullptr;
}

void CCamera::Init(float posx,float posy,float posz,float lookx,float looky,float lookz)
{
	_Eye.x = posx;
	_Eye.y = posy;
	_Eye.z = posz;

	_Lookat.x = lookx;
	_Lookat.y = looky;
	_Lookat.z = lookz;

	Length = sqrt((lookx-posx)*(lookx-posx)+(looky-posy)*(looky-posy)+(lookz-posz)*(lookz-posz));

	Angle.x = atan2(looky-posy,lookz-posz);
	Angle.y = atan2(lookx-posx,lookz-posz);
	Angle.z = atan2(lookx-posx,looky-posy);

}

void CCamera::Init(const VECTOR3& eye,const VECTOR3& lookat)
{
	_Eye.x = eye.x;
	_Eye.y = eye.y;
	_Eye.z = eye.z;

	_Lookat.x = lookat.x;
	_Lookat.y = lookat.y;
	_Lookat.z = lookat.z;

	Length = sqrt((lookat.x - eye.x)*(lookat.x - eye.x) + (lookat.y - eye.y)*(lookat.y - eye.y) + (lookat.z - eye.z)*(lookat.z - eye.z));

	Angle.x = atan2(lookat.y - eye.y,lookat.z - eye.z);
	Angle.y = atan2(lookat.x - eye.x,lookat.z - eye.z);
	Angle.z = atan2(lookat.x - eye.x,lookat.y - eye.y);
}

void CCamera::Update(void)
{
	if(CMouse::GetTrigger(CMouse::MOUSE_LEFT))
	{
		ClickPosX = CMouse::GetPosX();
	}
	if(CMouse::GetPress(CMouse::MOUSE_LEFT))
	{
		float x = CMouse::GetPosX();
		float sub = ClickPosX-x;
		Angle.y -= sub/10000.0f;
	}
	if(CMouse::GetWheel()==CMouse::WHEEL_DOWN)
	{
		Length -= 20.0f;
	}
	else if(CMouse::GetWheel()==CMouse::WHEEL_UP)
	{
		Length += 20.0f;
	}
	_Eye.x = sinf(Angle.y)*Length+_Lookat.x;
	_Eye.z = cosf(Angle.y)*Length+_Lookat.z;
}

void CCamera::Set(void)
{
	//3Dの設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,SCREEN_WIDTH/SCREEN_HEIGHT,1.0f,1500.0f);
	//ビュー行列
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//カメラの設定
	
	gluLookAt(_Eye.x,_Eye.y,_Eye.z,//カメラ座標
		_Lookat.x,_Lookat.y,_Lookat.z,//注視点座標
		_UpVec.x,_UpVec.y,_UpVec.z);//上方向ベクトル
}

void CCamera::UpdateCur(void)
{
	if (CurrentCamera != nullptr)
	{
		CurrentCamera->Update();
	}
}

void CCamera::SetCur(void)
{
	if (CurrentCamera != nullptr)
	{
		CurrentCamera->Set();
	}
}

void CCamera::Release(void)
{
	if (CurrentCamera != nullptr)
	{
		delete CurrentCamera;
		CurrentCamera = nullptr;
	}
}