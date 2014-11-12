#include "Camera.h"
#include "Mouse.h"
#include <math.h>
float ClickPosX;
CCamera::CCamera()
{
	PosP.x = 
	PosP.y = 
	PosP.z = 0;
	
	PosR.x =
	PosR.y = 0;
	PosR.z = -50.0f;

	VecU.x =
	VecU.z = 0;
	VecU.y = 1.0f;
}

CCamera::~CCamera()
{
}

void CCamera::Init(float posx,float posy,float posz,float lookx,float looky,float lookz)
{
	PosP.x = posx;
	PosP.y = posy;
	PosP.z = posz;

	PosR.x = lookx;
	PosR.y = looky;
	PosR.z = lookz;

	Length = sqrt((lookx-posx)*(lookx-posx)+(looky-posy)*(looky-posy)+(lookz-posz)*(lookz-posz));

	Angle.x = atan2(looky-posy,lookz-posz);
	Angle.y = atan2(lookx-posx,lookz-posz);
	Angle.z = atan2(lookx-posx,looky-posy);

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
	PosP.x = sinf(Angle.y)*Length+PosR.x;
	PosP.z = cosf(Angle.y)*Length+PosR.z;
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
	
	gluLookAt(PosP.x,PosP.y,PosP.z,//カメラ座標
		PosR.x,PosR.y,PosR.z,//注視点座標
		VecU.x,VecU.y,VecU.z);//上方向ベクトル
}