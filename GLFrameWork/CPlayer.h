//------------------------------------------------------------------------------
//
// プレイヤー [CPlayer.h]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------------------------
#define PLAYER_MOVE_SPEED (0.5f)			// 移動にかかる係数
#define PLAYER_RELOAD_TIME (300)			// リロードまでのフレーム
#define PLAYER_LIFE (3)						// プレイヤー体力の最大値
#define BARREL_ROT_MAX (-60.0f)				// 砲身の最大角度
#define BARREL_ROT_MIN (0.0f)				// 砲身の最小角度
#define PLAYER_COLLISION_RADIUS (100.0f)	// 当たり判定用球の半径

//------------------------------------------------------------------------------
// ヘッダインクルード
//------------------------------------------------------------------------------
#include "Model.h"

//------------------------------------------------------------------------------
// 列挙体宣言
//------------------------------------------------------------------------------
typedef enum
{
	PLAYER_STATE_WAIT = 0,	// 停止中
	PLAYER_STATE_MOVE,		// 移動中
	PLAYER_STATE_DAMAGE,	// 非ダメージ
	PLAYER_STATE_DEATH,		// 死亡
	PLAYER_STATE_RESPAWN,	// 復活中
	PLAYER_STATE_MAX,		// 最大数の退避
}PLAYER_STATE;

//------------------------------------------------------------------------------
// 前方宣言
//------------------------------------------------------------------------------
class CBullet;

//------------------------------------------------------------------------------
// クラス宣言
//------------------------------------------------------------------------------
class CPlayer:public CModel
{
public:
	CPlayer(void);
	~CPlayer(void);

	void Init(void);
	void Update(void);

	// 現在の装填時間ゲッター
	int ReloadTimer(void){return _ReloadTimer;}

	// 体力ゲッター
	int PlayerLife(void){return _PlayerLife;}

	static CPlayer* Create(int modelID, const VECTOR3& pos, int PlayerID);

	// 弾ポインタゲッター
	CBullet* Bullet(void){return _Bullet;}

private:
	CModel* Barrel;		// 砲身
	CBullet* _Bullet;	// 弾
	VECTOR3 Movement;	// 移動量
	float Speed;		// 移動速度
	float BarrelRotX;	// 砲身のX軸回転量
	bool LaunchFlag;	// 弾発射フラグ（true : 使用 / false : 不使用）
	int _ReloadTimer;	// 現在の装填時間
	int _PlayerLife;	// 体力
	int PlayerID;		// プレイヤー判別用ID
};

//------------------------------------------------------------------------------
// EOF