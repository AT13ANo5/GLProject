//=============================================================================
//	Connectionファイル [ Connection.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================
#ifndef _CONNECTION_H_
#define _CONNECTION_H_

//*****************************************************************************
//	インクルード
//*****************************************************************************
#include "Scene.h"
class CPolygon2D;
class CPushStart;

//=============================================================================
//	クラス定義
//=============================================================================
class CConnection :public CScene
{
public:
	CConnection();
	~CConnection();
	void Init(void);
	void Uninit(void);
	void Update(void);

	static void setEntry(int _charNum);
 static void setTexHostPos(int _id);

private:

	void recvUpdate();
	void keyUpdate();

	static CPolygon2D* Logo;
	static CPolygon2D* backGround;
	static CPolygon2D** waitBackGround;
	static CPolygon2D** waitPlayer;
	static CPushStart* pushStart;
	static int HostID;
 static CPolygon2D* texPlayer;
 static unsigned short playerNum;

	static bool PlayerEntry[PLAYER_MAX];
};

#endif

//	EOF