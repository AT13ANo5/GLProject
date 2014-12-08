//=================================================================================
//�J�����N���X
//=================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=================================================================================
//�C���N���[�h
//=================================================================================
#include "main.h"

//�N���X��`
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();
	virtual void Init(float posx,float posy,float posz,float lookx,float looky,float lookz);
	virtual void Init(const VECTOR3& eye,const VECTOR3& lookat);
	virtual void Update(void);
	
	//�J�������Z�b�g����
	void Set(void);
	//���̃V�[���ō�����J�����ł������悤�ɏC��
	static void SetCur(void);
	static void UpdateCur(void);
	static void Release(void);

	void SetEye(float posx,float posy,float posz){_Eye.x = posx;_Eye.y = posy;_Eye.z = posz;}
	void SetEye(const VECTOR3& pos){_Eye = pos;}
	void SetLookat(float posx,float posy,float posz){_Lookat.x = posx;_Lookat.y = posy;_Lookat.z = posz;}
	void SetLookat(VECTOR3& pos){_Lookat = pos;}
	void SetUpVec(float vecx,float vecy,float vecz){_UpVec.x = vecx,_UpVec.y = vecy,_UpVec.z = vecz;}
	void SetUpVec(VECTOR3& vec){_UpVec = vec;}
	
	void AddEye(float posx,float posy,float posz){ _Eye.x += posx;_Eye.y += posy;_Eye.z += posz; }
	void AddEye(const VECTOR3& pos){ _Eye += pos; }
	void AddLookat(float posx,float posy,float posz){ _Lookat.x += posx;_Lookat.y += posy;_Lookat.z += posz; }
	void AddLookat(VECTOR3& pos){ _Lookat += pos; }
	void AddUpVec(float vecx,float vecy,float vecz){ _UpVec.x += vecx,_UpVec.y += vecy,_UpVec.z += vecz; }
	void AddUpVec(VECTOR3& vec){ _UpVec += vec; }

	VECTOR3 Eye(void){return _Eye;}
	VECTOR3 Lookat(void){return _Lookat;}
	VECTOR3 UpVec(void){return _UpVec;}

protected:
	VECTOR3 _Eye;
	VECTOR3 _Lookat;
	VECTOR3 _UpVec;
	VECTOR3 Rot;
	float Length;
	VECTOR3 Angle;
	static CCamera* CurrentCamera;
};


#endif