#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"
#include"UI.h"

class CBattleAreaCylinder;
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
class CShadow;

class CGame :public CScene
{
public:

	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);

	typedef enum {
		PHASE_NONE = 0,
		PHASE_3,
		PHASE_2,
		PHASE_1,
		PHASE_START,
		PHASE_MAX
	} PHSE;


	static void SetPlayerState(NET_DATA _netData, DATA_TYPE _dataType);
	static void subTimer();


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
	static const float	HEIGHT_PLAYER_TO_FIELD;		// プレイヤーと地面の差

	static const float	RADIUS_AREA_BATTLE;			// 戦闘エリア半径
	static const float	HEIGHT_WALL;				// 壁の高さ

	static const int	MAX_ROCK;					// 岩の数
	static const VECTOR3 ROCK_POSITION_LIST[];		// 岩の座標リスト
 static const VECTOR3 PLAYER_POSITION_LIST[PLAYER_MAX]; // プレイヤー座標リスト
 static const VECTOR3 PLAYER_ROTATION_LIST[PLAYER_MAX]; // プレイヤー角度リスト

	void CheckHitPlayer(void);						// 攻撃判定の当たり判定 プレイヤー
	void CheckHitRock(void);						// 攻撃判定の当たり判定 岩
	void PushBackCharacter(void);					// キャラクター同士の押し戻し
	void PushBackRock(void);						// キャラクターと岩の押し戻し
	void PushBackField(void);						// 地形の押し戻し
	void IsLandField(void);							// 地形との判定
	void PushBackObjectByField(CObject* pObject, float offsetY);	// オブジェクトの地形による押し戻し
	void PushBackBattleArea(void);					// 行動可能範囲の押し戻し
	bool NeedsSkipPlayer(CPlayer* pPlayer);			// プレイヤー判定スキップ
	bool NeedsSkipBullet(CPlayer* pPlayer);			// 砲弾判定スキップ
	void HitBulletToField(void);						// 着弾地点判定
	void StartCount(void);

	CMeshGround*    Ground;     // フィールド
	CMeshSphere*    Sky;        // 空
	static CPlayer**       Player;     // プレイヤー	CNumberManager* numberManager;
	CBattleAreaCylinder*	CylinderArea;			// 戦闘可能エリア
	static CUI*		UI;
	CModel**		ppRock_;	//	岩
	CShadow**		Shadow;		// 影

	PHSE				gamePhase;
	int					gamePhaseCnt;
};
#endif