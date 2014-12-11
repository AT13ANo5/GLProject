#ifndef _RESULT_H_
#define _RESULT_H_
#include "Scene.h"

class CModel;
class CPolygon2D;
class CMeshGround;
class CMeshSphere;

class CResult :public CScene
{
public:
	CResult();
	~CResult();

	void Init(void);
	void Uninit(void);
	void Update(void);

private:
  static const float  RADIUS_SKY;   // ‹ó‚Ì”¼Œa

  CPolygon2D*  Logo;
  CModel*      Player;
  CMeshGround* Ground;   // ’nŒ`
  CMeshSphere* Sky;      // ‹ó
};
#endif