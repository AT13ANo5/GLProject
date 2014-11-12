#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

class CCamera
{
public:
	CCamera();
	virtual ~CCamera();
	void Init(float posx,float posy,float posz,float lookx,float looky,float lookz);
	void Update(void);
	void Set(void);
	void SetPosP(float posx,float posy,float posz){PosP.x = posx;PosP.y = posy;PosP.z = posz;}
	void SetPosR(float posx,float posy,float posz){PosR.x = posx;PosR.y = posy;PosR.z = posz;}
	VECTOR3 GetPosP(void){return PosP;}
	VECTOR3 GetPosR(void){return PosR;}
private:
	VECTOR3 PosP;
	VECTOR3 PosR;
	VECTOR3 VecU;
	VECTOR3 Rot;
	float Length;
	VECTOR3 Angle;
};


#endif