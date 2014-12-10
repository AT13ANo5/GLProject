#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "Scene.h"

class CPolygon2D;
class CModel;

class CTitle :public CScene
{
public:
	CTitle();
	~CTitle();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:

	CPolygon2D* Logo;
  CModel*     Player;
  CModel* Balel;

};
#endif