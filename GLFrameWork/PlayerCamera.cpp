#include "PlayerCamera.h"
#include "Object.h"
#include "SoundAL.h"
#include "Input\Keyboard.h"
#include<math.h>
#define CAMERA_HEIHGT (20.0f)
#define CAMERA_SPEED (2)
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
	VECTOR3 rot = Rot = Parent->Rot();
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
 cameraHeight = CAMERA_HEIHGT;
 CameraMove = false;
 cameraControl = true;
}

void CPlayerCamera::Update(void)
{
 if(CameraMode)
 {
  if(cameraControl)
  {
   cameraMotion();
  }
 }
 else{
  VECTOR3 rot = Rot = Parent->Rot();
  VECTOR3 pos = Parent->Pos();
  _Eye.x = sinf(DEG2RAD(rot.y) + PI)*Length;
  DestEye.y = Parent->Pos().y + CAMERA_HEIHGT;
  _Eye.z = cosf(DEG2RAD(rot.y) + PI)*Length;
  _Eye.x += pos.x;
  _Eye.z += pos.z;

  _Eye.y += ( DestEye.y - _Eye.y )*0.5f;

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
}

void CPlayerCamera::cameraMotion(void)
{
 if(CameraMove || CKeyboard::Instance()->GetTrigger(DIK_3) || CKeyboard::Instance()->GetTrigger(DIK_4) || CKeyboard::Instance()->GetTrigger(DIK_5))
 {
  VECTOR3 rot = Rot = Parent->Rot();
  VECTOR3 pos = Parent->Pos();
  _Eye.x = sinf(DEG2RAD(rot.y) + PI)*Length;
  DestEye.y = Parent->Pos().y + CAMERA_HEIHGT;
  _Eye.z = cosf(DEG2RAD(rot.y) + PI)*Length;
  _Eye.x += pos.x;
  _Eye.z += pos.z;

  _Eye.y += ( DestEye.y - _Eye.y )*0.5f;

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

 if(CKeyboard::Instance()->GetPress(DIK_T))
 {
  _Eye.y += 2;
  _Lookat.y += 2;
 }
 if(CKeyboard::Instance()->GetPress(DIK_G))
 {
  _Eye.y -= 2;
  _Lookat.y -= 2;
 }
 if(CKeyboard::Instance()->GetPress(DIK_Y))
 {
  _Eye.y += 2;
 }
 if(CKeyboard::Instance()->GetPress(DIK_H))
 {
  _Eye.y -= 2;
 }
 if(CKeyboard::Instance()->GetPress(DIK_U))
 {
  _Lookat.y += 2;
 }
 if(CKeyboard::Instance()->GetPress(DIK_J))
 {
  _Lookat.y -= 2;
 }

 if(CKeyboard::Instance()->GetPress(DIK_Q))
 {
  Rot.y += 2;
  REVISE_PI_DEG(Rot.y);
  _Eye.x = _Lookat.x - ( Length * sinf(DEG2RAD(Rot.y)) );
  _Eye.z = _Lookat.z - ( Length * cosf(DEG2RAD(Rot.y)) );

 }

 if(CKeyboard::Instance()->GetPress(DIK_E))
 {
  Rot.y -= 2;
  REVISE_PI_DEG(Rot.y);
  _Eye.x = _Lookat.x -( Length * sinf(DEG2RAD(Rot.y)));
  _Eye.z = _Lookat.z -( Length * cosf(DEG2RAD(Rot.y)));
 }
 if(CKeyboard::Instance()->GetPress(DIK_Z))
 {
  Rot.y += 2;
  REVISE_PI_DEG(Rot.y);
  _Lookat.x = _Eye.x + sinf(DEG2RAD(Rot.y))*Length;
  _Lookat.z = _Eye.z + cosf(DEG2RAD(Rot.y))*Length;
 }

 if(CKeyboard::Instance()->GetPress(DIK_X))
 {
  Rot.y -= 2;
  REVISE_PI_DEG(Rot.y);
  _Lookat.x = _Eye.x + sinf(DEG2RAD(Rot.y))*Length;
  _Lookat.z = _Eye.z + cosf(DEG2RAD(Rot.y))*Length;
 }

 if(CKeyboard::Instance()->GetPress(DIK_S))
 {
  _Eye.x -= sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Eye.z -= cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;

  _Lookat.x -= sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Lookat.z -= cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;
 }

 if(CKeyboard::Instance()->GetPress(DIK_W))
 {
  _Eye.x += sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Eye.z += cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;

  _Lookat.x += sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Lookat.z += cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;
 }
 if(CKeyboard::Instance()->GetPress(DIK_A))
 {
  _Eye.x += cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Eye.z -= sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;

  _Lookat.x += cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Lookat.z -= sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;
 }
 if(CKeyboard::Instance()->GetPress(DIK_D))
 {
  _Eye.x -= cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Eye.z += sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;

  _Lookat.x -= cosf(DEG2RAD(Rot.y))*CAMERA_SPEED;
  _Lookat.z += sinf(DEG2RAD(Rot.y))*CAMERA_SPEED;
 }
 DestEye = _Eye;

}