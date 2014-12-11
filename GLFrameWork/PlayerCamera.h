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

private:
	CObject* Parent;
	
};


#endif