#include "PlayerCamera.h"
#include "Object.h"
#include<math.h>
CPlayerCamera* CPlayerCamera::Create(CObject* parent,float r)
{
	if (parent == nullptr){return nullptr;}

	CPlayerCamera* camera = new CPlayerCamera;
	if (camera == nullptr){return nullptr;}

	camera->Length = r;
	camera->Parent = parent;
	camera->Init();

	return camera;
}

void CPlayerCamera::Init(void)
{
	VECTOR3 rot = Parent->Rot();
	VECTOR3 pos = Parent->Pos();
	_Eye.x = sinf(DEG2RAD(rot.y)+PI)*Length;
	_Eye.y = Parent->Pos().y+50.0f;
	_Eye.z = cosf(DEG2RAD(rot.y)+PI)*Length;
	_Eye += pos;

	_Lookat.x = sinf(DEG2RAD(rot.y))*Length;
	_Lookat.y = _Eye.y;
	_Lookat.z = cosf(DEG2RAD(rot.y))*Length;
	_Lookat += pos;

}

void CPlayerCamera::Update(void)
{
	VECTOR3 rot = Parent->Rot();
	VECTOR3 pos = Parent->Pos();
	_Eye.x = sinf(DEG2RAD(rot.y)+PI)*Length;
	_Eye.y = Parent->Pos().y + 50.0f;
	_Eye.z = cosf(DEG2RAD(rot.y)+PI)*Length;
	_Eye += pos;

	_Lookat.x = sinf(DEG2RAD(rot.y))*Length;
	_Lookat.y = _Eye.y;
	_Lookat.z = cosf(DEG2RAD(rot.y))*Length;
	_Lookat += pos;
}