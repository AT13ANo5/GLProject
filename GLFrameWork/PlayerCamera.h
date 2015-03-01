#ifndef _PLAYER_CAMERA_H_
#define _PLAYER_CAMERA_H_
#include "main.h"
#include "Camera.h"

class CObject;

class CPlayerCamera :public CCamera
{
public:
	static CPlayerCamera* Create(CObject* parent,float r);

	void Init(void);
	void Update(void);

 void ChangeParent(CObject* parent){ Parent = parent; }
 void SetCameraMode(bool cameraMode){ CameraMode = cameraMode; }
 void SetCameraControl(bool cameracontrol){ cameraControl = cameracontrol; }
private:
void cameraMotion(void);
	CObject* Parent;
	VECTOR3 DestEye;
 bool CameraMode;
 bool CameraMove;
 bool cameraControl;
 float cameraHeight;
};


#endif