//kikushima takahiro ��폜

#ifndef _TITLE_H_
#define _TITLE_H_
#include "main.h"
#include "Scene.h"


class CPolygon2D;
class CModel;
class CPlayer;
class CMeshGround;
class CMeshSphere;
class CCamera;
class CPushStart;
class CTitleDirection;

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

  CPushStart* PushEnter;
  CPlayer**    Player;
  CModel** Rock;
  CMeshGround*  Ground;   // �n�`
  CMeshSphere*  Sky;      // ��
  CCamera* Camera;
  CTitleDirection* TitleD;

  float CameraRotation;
  float MoveSpeed;
  float RotSpeed;
  float MoveRot;
};
#endif