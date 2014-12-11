#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "Scene.h"

class CPolygon2D;
class CPushStart;

class CConnection :public CScene
{
public:
	CConnection();
	~CConnection();

	void Init(void);
	void Uninit(void);
	void Update(void);
private:

	CPolygon2D* Logo;
	CPolygon2D* backGround;
	CPolygon2D** waitBackGround;
	CPolygon2D** waitPlayer;

	CPushStart* pushStart;
};

#endif