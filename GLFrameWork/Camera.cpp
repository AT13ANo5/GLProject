#include "Camera.h"
#include "Mouse.h"
#include <math.h>

float ClickPosX;

CCamera* CCamera::Top = nullptr;
CCamera* CCamera::Cur = nullptr;

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

	LinkList();
}

CCamera::~CCamera()
{
	UnlinkList();
}

//=============================================================================
//自身をリストに追加
//=============================================================================
void CCamera::LinkList(void)
{
	if (Top != nullptr)//二つ目以降の処理
	{
		CCamera* pScene = Cur;
		pScene->Next = this;
		Prev = pScene;
		Next = nullptr;
		Cur = this;
	}
	else//最初の一つの時の処理
	{
		Top = this;
		Cur = this;
		Prev = nullptr;
		Next = nullptr;
	}
}

//=============================================================================
//自身をリストから削除
//=============================================================================
void CCamera::UnlinkList(void)
{
	if (Prev == nullptr)//先頭
	{
		if (Next != nullptr)//次がある
		{
			Next->Prev = nullptr;
			Top = Next;
		}
		else//最後の一つだった
		{
			Top = nullptr;
			Cur = nullptr;
		}
	}
	else if (Next == nullptr)//終端
	{
		if (Prev != nullptr)//前がある
		{
			Prev->Next = nullptr;
			Cur = Prev;
		}
		else//最後の一つだった
		{
			Top = nullptr;
			Cur = nullptr;
		}
	}
	else//前後にデータがあるとき
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = nullptr;
	Next = nullptr;
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

void CCamera::UpdateAll(void)
{
	CCamera* camera = Top;

	while (camera)
	{
		camera->Update();
		camera = camera->Next;
	}
}

void CCamera::Set(void)
{
	//3Dの設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,SCREEN_WIDTH/SCREEN_HEIGHT,1.0f,3000.0f);
	//ビュー行列
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//カメラの設定
	
	gluLookAt(_Eye.x,_Eye.y,_Eye.z,//カメラ座標
		_Lookat.x,_Lookat.y,_Lookat.z,//注視点座標
		_UpVec.x,_UpVec.y,_UpVec.z);//上方向ベクトル
}

void CCamera::Set(int num)
{
	CCamera* camera = Top;
	int cnt = 0;

	while (camera)
	{
		if (cnt == num)
		{
			camera->Set();
			break;
		}
		camera = camera->Next;
		cnt++;
	}
}

void CCamera::ReleaseAll(void)
{
	CCamera* camera = Top;
	CCamera* next = nullptr;
	while (camera)
	{
		next = camera->Next;
		delete camera;
		camera = nullptr;
		camera = next;
	}
}

CCamera* CCamera::Camera(int num)
{
	CCamera* camera = Top;
	int cnt = 0;

	while (camera)
	{
		if (cnt == num)
		{
			return camera;
		}
		camera = camera->Next;
		cnt++;
	}
	return nullptr;
}