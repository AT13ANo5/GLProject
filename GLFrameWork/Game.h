#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CMeshGround;
class CMeshSphere;
class CModel;

class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
private:
  static const float  RADIUS_SKY;   // ��̔��a

  CMeshGround*  Ground;   // �n�`
  CMeshSphere*  Sky;      // ��
  CModel*       Player;   // �v���C���[
};

#endif