//=============================================================================
//	Connection�t�@�C�� [ Connection.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

//*****************************************************************************
//	�C���N���[�h
//*****************************************************************************
#include "Scene.h"
class CPolygon2D;
class CPushStart;

//=============================================================================
//	�N���X��`
//=============================================================================
class CConnection :public CScene
{
public:
	CConnection();
	~CConnection();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:

	void recvUpdate();
	void keyUpdate();

	CPolygon2D* Logo;
	CPolygon2D* backGround;
	CPolygon2D** waitBackGround;
	CPolygon2D** waitPlayer;
	CPushStart* pushStart;
};

#endif

//	EOF