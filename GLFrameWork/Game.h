#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CMeshGround;
class CMeshSphere;
class CModel;
class CPlayer;
class CLoadGauge;
class CLoadString;
class CReport;
class CNumberManager;class CMiniMap;
class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
private:
	static const float  RADIUS_SKY;					// 空の半径
	static const float	RADIUS_DEFENSE_CHARACTER;	// キャラクターの防御半径
	static const float	RADIUS_OFFENSE_BULLET;		// 砲弾の攻撃半径
	static const float	RADIUS_PUSH_CHARACTER;		// キャラクターの押し戻し半径

	void CheckHit(void);				// 攻撃判定の当たり判定
	void PushBackCharacter(void);		// キャラクター同士の押し戻し
	void PushBackField(void);			// 地形の押し戻し

	CMeshGround*    Ground;     // フィールド
	CMeshSphere*    Sky;        // 空
	CPlayer*        Player;     // プレイヤー
	CLoadGauge*     loadGauge;  // 装填ゲージ
	CLoadString*    loadString; // 装填中の文字
	CReport*        reportBg;
	CReport*        report;
	CNumberManager* numberManager;
	CMiniMap*	MiniMap;

};
#endif