#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"
#include"UI.h"

class CMeshCylinder;
class CMeshGround;
class CMeshSphere;
class CModel;
class CObject;
class CPlayer;
class CLoadGauge;
class CLoadString;
class CReport;
class CNumberManager;
class UI;

class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);

	static void SetPlayerState(NET_DATA _netData, DATA_TYPE _dataType);

private:
	static const float  RADIUS_SKY;					// 空の半径
	static const float	RADIUS_DEFENSE_CHARACTER;	// キャラクターの防御半径
	static const float	HEIGHT_DEFENSE_CHARACTER;	// キャラクターの防御中心高さ
	static const float	RADIUS_OFFENSE_BULLET;		// 砲弾の攻撃半径
	static const float	HEIGHT_OFFENSE_BULLET;		// 砲弾の攻撃中心高さ
	static const float	RADIUS_PUSH_CHARACTER;		// キャラクターの押し戻し半径
	static const float	HEIGHT_PUSH_CHARACTER;		// キャラクターの押し戻し中心高さ
	static const float	RADIUS_DEFENSE_ROCK;		// 岩の防御半径
	static const float	HEIGHT_DEFENSE_ROCK;		// 岩の防御中心高さ
	static const float	RADIUS_PUSH_ROCK;			// 岩の押し戻し半径
	static const float	HEIGHT_PUSH_ROCK;			// 岩の押し戻し中心高さ
	static const float FIELD_PANEL_SIZE;			//フィールドのパネル一枚のサイズ

	static const float	RADIUS_AREA_BATTLE;			// 戦闘エリア半径

	static const int	MAX_ROCK;					// 岩の数

	void CheckHitPlayer(void);						// 攻撃判定の当たり判定 プレイヤー
	void CheckHitRock(void);						// 攻撃判定の当たり判定 岩
	void PushBackCharacter(void);					// キャラクター同士の押し戻し
	void PushBackRock(void);						// キャラクターと岩の押し戻し
	void PushBackField(void);						// 地形の押し戻し
	void IsLandField(void);							// 地形との判定
	void PushBackObjectByField(CObject* pObject);	// オブジェクトの地形による押し戻し
	void PushBackBattleArea(void);					// 行動可能範囲の押し戻し
	bool NeedsSkipPlayer(CPlayer* pPlayer);			// プレイヤー判定スキップ
	bool NeedsSkipBullet(CPlayer* pPlayer);			// 砲弾判定スキップ

	CMeshGround*    Ground;     // フィールド
	CMeshSphere*    Sky;        // 空
	static CPlayer**       Player;     // プレイヤー	CNumberManager* numberManager;
	CMeshCylinder*	CylinderArea;	// 戦闘可能エリア
	//CReport*        reportBg;
	//CReport*        report;
	//CNumberManager* numberManager;
	CUI*		UI;

	CModel**		ppRock_;	//	岩

};
#endif