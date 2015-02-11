//------------------------------------------------------------------------------
//
// プレイヤー [CPlayer.cpp]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ヘッダインクルード
//------------------------------------------------------------------------------
#include <math.h>

#include "main.h"
#include "CPlayer.h"
#include "Keyboard.h"
#include "Camera.h"
#include "CBullet.h"
#include "CommonGL.h"
#include "Ballistic.h"
#include "ManagerGL.h"
#include "Explosion.h"
const int kHeightMax = 400;
const int kUpSpeed = 3;
const int kDamageCntMax = 60;
const float BARREL_ROTX_SPEED = 1.0f;
const float PLAYER_ROTY_SPEED = 3.0f;
//------------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------------
// 引数
//  なし
//------------------------------------------------------------------------------
CPlayer::CPlayer() :CModel()
{
	// 移動速度
	Speed = PLAYER_MOVE_SPEED;

	// 弾初期化
	_Bullet = nullptr;

	// 装填時間
	_ReloadTimer = PLAYER_RELOAD_TIME;

	// 砲身のX軸回転量
	BarrelRotX = 0.0f;


	killCount = 0;
	deathCount = 0;
}

//------------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------------
// 引数
//  なし
//------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	SafeDelete(Ballistic);
	SafeDelete(_Feed);
}

//------------------------------------------------------------------------------
// 初期化
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	// 使用フラグ
	PlayerFlag = false;

	// 弾使用フラグ
	_BulletUseFlag = false;

	LaunchFlag = false;

	// 弾
	_Bullet = nullptr;

	// 弾道

	Ballistic = CBallistic::Create(COLOR(1.0f,0.0f,0.0f,0.3f));

	// 継承元の初期化
	CModel::Init();

	// 砲身
	Barrel = CModel::Create(TANK_BARREL,_Pos);
	Barrel->Init();
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_YELLOW));


	//高さ初期化
	_Hegiht = 0;
	// 体力
	_PlayerLife = PLAYER_LIFE;
	_Feed = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	_Feed->SetColor(COLOR(0,0,0,0));
	_Timer = 0;
}

//------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::Update()
{
	//モード選択
	switch (_State)
	{
	case PLAYER_STATE_DEATH:
	{
		AddPosY(kUpSpeed+0.0f);
		_Hegiht += kUpSpeed;
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;

		if (PlayerID == CManager::netData.charNum)
			_Feed->AddAlpha(Alpha);



		Barrel->SetPos(_Pos);			// 位置
		CManager::SendPos(_Pos);
		CManager::SendRot(_Rot);
		CManager::SendCannonRot(Barrel->Rot());
		CManager::SendCannon(LaunchFlag);





		if (_Hegiht > kHeightMax)
		{
			SetRespawn();
		}
		return;
	}
	case PLAYER_STATE_RESPAWN:
	{
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;

		AddPosY(-kUpSpeed);
		_Hegiht += kUpSpeed;


		if (PlayerID == CManager::netData.charNum)
			_Feed->AddAlpha(-Alpha);





		Barrel->SetPos(_Pos);			// 位置
		CManager::SendPos(_Pos);
		CManager::SendRot(_Rot);
		CManager::SendCannonRot(Barrel->Rot());
		CManager::SendCannon(LaunchFlag);






		if (_Hegiht > kHeightMax)
		{
			_State = PLAYER_STATE_WAIT;
			_Feed->SetAlpha(0);
		}
		return;
	}
	case PLAYER_STATE_DAMAGE:
	{
		_Timer++;
		_State = PLAYER_STATE_WAIT;
		break;
	}
	default:
		break;
	}

	if (_Timer != 0)
	{
		_Timer++;
		if (_Timer % 2 == 0)
		{
			Barrel->SetDrawFlag(true);
			SetDrawFlag(true);
		}
		else{
			Barrel->SetDrawFlag(false);
			SetDrawFlag(false);
		}
		if (_Timer > kDamageCntMax)
		{
			Barrel->SetDrawFlag(true);
			SetDrawFlag(true);
			_Timer = 0;
		}

	}

	if (PlayerFlag == true)
	{
		// 操作キャラクターの更新
		UpdatePlayer();
	}
	else
	{
		// 操作キャラクターではない＝CPU
		UpdateCPU();
	}
}

//------------------------------------------------------------------------------
// キャラクターが操作キャラだった場合の更新
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::UpdatePlayer(void)
{
	// 弾の使用状態を確定
	if (_Bullet == nullptr)
	{
		_BulletUseFlag = false;
	}

	// 初期化
	VECTOR3 rot = VECTOR3(0.0f,0.0f,0.0f);

	// 移動
	// 上
	if (CKeyboard::GetPress(DIK_W))
	{
		Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed;
		Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed;
	}

	// 下
	else if (CKeyboard::GetPress(DIK_S))
	{
		Movement.x -= sinf(DEG2RAD(_Rot.y)) * Speed;
		Movement.z -= cosf(DEG2RAD(_Rot.y)) * Speed;
	}

	// 左
	if (CKeyboard::GetPress(DIK_A))
	{
		rot.y += PLAYER_ROTY_SPEED;
	}

	// 右
	else if (CKeyboard::GetPress(DIK_D))
	{
		rot.y -= PLAYER_ROTY_SPEED;
	}

	// 砲身の上下
	if (CKeyboard::GetPress(DIK_UP))
	{
		BarrelRotX -= BARREL_ROTX_SPEED;
	}
	else if (CKeyboard::GetPress(DIK_DOWN))
	{
		BarrelRotX += BARREL_ROTX_SPEED;
	}

	// キャラクターの回転
	AddRot(rot);

	// 値の丸め込み
	// プレイヤーの回転量
	if (Rot().y > 360.0f)
	{
		SetRotY(Rot().y - 2 * 360.0f);
	}
	else if (Rot().y < -360.0f)
	{
		SetRotY(Rot().y + 2 * 360.0f);
	}

	// 砲身
	if (BarrelRotX > BARREL_ROT_MIN)
	{
		BarrelRotX = BARREL_ROT_MIN;
	}
	else if (BarrelRotX < BARREL_ROT_MAX)
	{
		BarrelRotX = BARREL_ROT_MAX;
	}

	// キャラクターの移動値を加算
	AddPos(Movement);

	// 減速
	Movement *= 0.95f;

	// 砲身の位置、回転を更新
	Barrel->SetPos(_Pos);			// 位置
	Barrel->SetRot(_Rot);			// 回転
	Barrel->AddRotX(BarrelRotX);	// 上で設定した回転量に砲身のX軸回転量を加算

	// 弾道の更新
	Ballistic->Update(_Pos,VECTOR3(BarrelRotX,_Rot.y,_Rot.z));

	// 弾の発射
	// 弾が発射されていなかった時
	if (LaunchFlag == false)
	{
		if (CKeyboard::GetTrigger(DIK_SPACE))
		{
			_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
			LaunchFlag = true;
			_BulletUseFlag = true;
			_ReloadTimer = 0;
			CManager::SendCannon(LaunchFlag);
		}
	}
	// 弾が発射されている時
	else
	{
		// リロード可能までの時間を加算
		_ReloadTimer++;

		// リロード制限時間を超えたら
		if (_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			// 再発射可能に
			LaunchFlag = false;
			_BulletUseFlag = false;
			_ReloadTimer = PLAYER_RELOAD_TIME;
		}
	}

	CManager::SendPos(_Pos);
	CManager::SendRot(_Rot);
	CManager::SendCannonRot(Barrel->Rot());

#ifdef _DEBUG
	// デバッグ用
	// 連射
	if (CKeyboard::GetPress(DIK_V))
	{
		CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
	}

	// ライフの減算
	if (CKeyboard::GetPress(DIK_L))
	{
		this->AddPlayerLife(-1);
		_State = PLAYER_STATE_DAMAGE;
	}

	// 弾の削除確認
	if (CKeyboard::GetPress(DIK_M))
	{
		this->ReleaseBullet();
	}

#endif
}

//=============================================================================
//	弾発射処理
//=============================================================================
void CPlayer::BlastBullet()
{
	if (LaunchFlag == false)
	{
		_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
		LaunchFlag = true;
		_BulletUseFlag = true;
		_ReloadTimer = 0;
	}
}

//------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::UpdateCPU(void)
{
	Barrel->SetPos(_Pos);			// 位置
	BarrelRotX = Barrel->Rot().x;
	// 弾が発射されていなかった時
	if (LaunchFlag == true)
	{
		// リロード可能までの時間を加算
		_ReloadTimer++;

		// リロード制限時間を超えたら
		if (_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			// 再発射可能に
			LaunchFlag = false;
			_BulletUseFlag = false;
			_ReloadTimer = PLAYER_RELOAD_TIME;
		}
	}
}

void CPlayer::setBarrelRot(VECTOR3 _rot)
{
	Barrel->SetRot(_rot);			// 回転
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::ReleaseBullet(void)
{
	// 弾を使用していたら削除
	if (_Bullet != nullptr)
	{
		SafeRelease(_Bullet);
		_BulletUseFlag = false;
	}
}

//------------------------------------------------------------------------------
// 生成
//------------------------------------------------------------------------------
// 引数
//  modelID		: 生成したいモデルのID
//  pos			: 初期位置
//  playerID	: プレイヤーのID（通し番号）
// 戻り値
//  CPlayer	: 生成したプレイヤーのポインタ
//------------------------------------------------------------------------------
CPlayer* CPlayer::Create(int modelID,const VECTOR3& pos,int playerID)
{
	CPlayer* model = new CPlayer;

	if (model == nullptr)
	{
		return nullptr;
	}

	model->ModelID = modelID;
	model->PlayerID = playerID;
	model->_Pos = pos;
	model->Init();

	return model;
}
//------------------------------------------------------------------------------
// 死亡処理
//------------------------------------------------------------------------------
// 引数
// なし
// 戻り値
// なし
//------------------------------------------------------------------------------
void CPlayer::SetDeath(VECTOR3 pos, int _charNum)
{
	if (_State != PLAYER_STATE_DEATH)
	{
		_Hegiht = 0;
		_State = PLAYER_STATE_DEATH;
		_PlayerRespown = pos;


		if (_charNum == CManager::netData.charNum)
			_Feed->SetAlpha(0);
	}
}
//------------------------------------------------------------------------------
// 復活処理
//------------------------------------------------------------------------------
// 引数
// pos			: 初期位置
// 戻り値
// なし
//------------------------------------------------------------------------------
void CPlayer::SetRespawn(void)
{
	_Hegiht = 0;
	_State = PLAYER_STATE_RESPAWN;
	_PlayerRespown.y += kHeightMax;
	SetPos(_PlayerRespown);
	_PlayerLife = PLAYER_LIFE;

	if (PlayerID == CManager::netData.charNum)
		_Feed->SetAlpha(1);
	
	Movement = VECTOR3(0,0,0);



	Barrel->SetPos(_Pos);			// 位置
	CManager::SendPos(_Pos);
	CManager::SendRot(_Rot);
	CManager::SendCannonRot(Barrel->Rot());
	CManager::SendCannon(LaunchFlag);

}
//------------------------------------------------------------------------------
// 加算処理
//------------------------------------------------------------------------------
// 引数
// addVal			: ライフ
// 戻り値
// なし
//------------------------------------------------------------------------------

void CPlayer::AddPlayerLife(int addVal)
{
	if (_Timer != 0 || _State != PLAYER_STATE_DAMAGE)
	{
		_PlayerLife += addVal;
		if (_PlayerLife < 0)
		{
			_PlayerLife = 0;
		}
		else
			if (_PlayerLife > PLAYER_LIFE)
			{
				_PlayerLife = PLAYER_LIFE;
			}
	}
}

//------------------------------------------------------------------------------
// EOF