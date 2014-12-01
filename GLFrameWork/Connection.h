#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "Scene.h"

class CPolygon2D;

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
};

#endif