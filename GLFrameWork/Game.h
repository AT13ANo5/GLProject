#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CMeshGround;
class CMeshSphere;
class CModel;
class CPlayer;
class CLoadGauge;
class CLoadString;

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

	CMeshGround*  Ground;     // フィールド
	CMeshSphere*  Sky;        // 空
	CPlayer*      Player;     // プレイヤー
	CLoadGauge*   loadGauge;  // 装填ゲージ
	CLoadString*  loadString; // 装填中の文字
};

#endif