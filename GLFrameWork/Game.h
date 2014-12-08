#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CMeshGround;
class CMeshSphere;
class CModel;
class CPlayer;

class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
private:
  static const float  RADIUS_SKY;   // 空の半径

  CMeshGround*  Ground;   // 地形
  CMeshSphere*  Sky;      // 空
  CPlayer*      Player;   // プレイヤー
};

#endif