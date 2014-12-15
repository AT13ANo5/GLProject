#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "Scene.h"


class CPolygon2D;
class CModel;
class CMeshGround;
class CMeshSphere;
class CCamera;

class CTitle :public CScene
{
public:
	CTitle();
	~CTitle();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
  static const float  RADIUS_SKY;   // ��̔��a

	CPolygon2D* Logo;
  CModel*     Player;
  CModel* Barrel;
  CModel* Rock;
  CMeshGround*  Ground;   // �n�`
  CMeshSphere*  Sky;      // ��
  CCamera* Camera;
  float CameraRotation;
};
#endif