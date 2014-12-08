#ifndef _SPLASH_H_
#define _SPLASH_H_
#include "Scene.h"

class CPolygon2D;

class CSplash :public CScene
{
public:
	CSplash();
	~CSplash();

	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	CPolygon2D* Logo;
};

#endif