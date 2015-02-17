#include "PlayerCamera.h"
#include "Object.h"
#include "SoundAL.h"
#include<math.h>
#define CAMERA_HEIHGT (20.0f)
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
	_Eye.y = Parent->Pos().y+CAMERA_HEIHGT;
	_Eye.z = cosf(DEG2RAD(rot.y)+PI)*Length;
	_Eye += pos;

	_Lookat.x = sinf(DEG2RAD(rot.y))*Length;
	_Lookat.y = Parent->Pos().y;
	_Lookat.z = cosf(DEG2RAD(rot.y))*Length;
	_Lookat += pos;

	DestEye = _Eye;
	VECTOR3 vec;
	vec.x = sinf(DEG2RAD(rot.y));
	vec.y = 0;
	vec.z = cosf(DEG2RAD(rot.y));
	vec.Normalize();
	CSoundAL::SetListenerPos(_Eye);
	CSoundAL::SetListenerFVec(vec);
	CSoundAL::SetListenerUVec(VECTOR3(0,1.0f,0));
	CSoundAL::SetDefaultMaxDistance(400.0f);

}

void CPlayerCamera::Update(void)
{
	VECTOR3 rot = Parent->Rot();
	VECTOR3 pos = Parent->Pos();
	_Eye.x = sinf(DEG2RAD(rot.y)+PI)*Length;
	DestEye.y = Parent->Pos().y + CAMERA_HEIHGT;
	_Eye.z = cosf(DEG2RAD(rot.y)+PI)*Length;
	_Eye.x += pos.x;
	_Eye.z += pos.z;

	_Eye.y += (DestEye.y - _Eye.y)*0.5f;

	_Lookat.x = sinf(DEG2RAD(rot.y))*Length;
	_Lookat.y = 0;
	_Lookat.z = cosf(DEG2RAD(rot.y))*Length;
	_Lookat += pos;

	VECTOR3 vec;
	vec.x = sinf(DEG2RAD(rot.y));
	vec.y = 0;
	vec.z = cosf(DEG2RAD(rot.y));
	vec.Normalize();

	CSoundAL::SetListenerPos(_Eye);
	CSoundAL::SetListenerFVec(vec);
}