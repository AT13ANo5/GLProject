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
#include "Camera.h"
#include "CBullet.h"
#include "CommonGL.h"
#include "ManagerGL.h"
#include "Explosion.h"
#include "SandCloud.h"
#include "SoundAL.h"
#include "Input\VC.h"
#include "Input\Keyboard.h"

#define NARI_SCL (30.0f)
const int kHeightMax = 400;
const int kUpSpeed = 3;
const int kDamageCntMax = 60;
const float BARREL_ROTX_SPEED = 1.0f;
const float PLAYER_ROTY_SPEED = 1.0f;
const int NARI_SCL_SPEED = 6;
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

	DriveSE = nullptr;
	IdlingSE = nullptr;

	// 装填時間
	_ReloadTimer = PLAYER_RELOAD_TIME;

	// 砲身のX軸回転量
	BarrelRotX = 0.0f;


	killCount = 0;
	deathCount = 0;
	_SandTime = 0;
}

//------------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------------
// 引数
//  なし
//------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	SafeRelease(DriveSE);
	SafeRelease(IdlingSE);
	SafeDelete(Ballistic);
	SafeDelete(_Feed);
	SafeRelease(_nari);
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

	// 弾発射フラグ
	_LaunchFlag = false;

	// ゲーム終了フラグ
	_EndGameFlag = false;

	// 弾
	_Bullet = nullptr;

	// 弾道
	Ballistic = nullptr;

	// 継承元の初期化
	CModel::Init();

	// 砲身
	Barrel = CModel::Create(TANK_BARREL,_Pos);
	Barrel->Init();
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_YELLOW));

	_nari = CBillboard::Create(_Pos,VECTOR2(512 / NARI_SCL,1024 / NARI_SCL));
	_nari->SetTex(CTexture::Texture(TEX_NARITADA));
	_nari->SetAlpha(0.0f);

	//高さ初期化
	_Hegiht = 0;

	// 体力
	_PlayerLife = PLAYER_LIFE;
	_Feed = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	_Feed->SetColor(COLOR(0,0,0,0));
	_Timer = 0;

	_NariSclSpeed = NARI_SCL_SPEED;
	DriveSE = CSoundAL::Play(CSoundAL::SE_DRIVE,_Pos);
	DriveSE->SetVolume(0);
	IdlingSE = CSoundAL::Play(CSoundAL::SE_IDLING,_Pos);

	_InputFlag = true;
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

	DriveSE->SetPos(_Pos);
	IdlingSE->SetPos(_Pos);
	//モード選択
	switch (_State)
	{
	case PLAYER_STATE_DEATH:
	{
		AddPosY(kUpSpeed + 0.0f);
		_Hegiht += kUpSpeed;
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;



		if (PlayerID == CManager::netData.charNum)
		{
			_Feed->AddAlpha(Alpha);
		}
  VECTOR3 pos = _Pos;
  pos.y += 10;
  pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
  pos.z += cosf(DEG2RAD(_Rot.y)) * -15;

  _nari->SetPos(pos);
		_nari->AddAlpha(Alpha * 3);
		Barrel->SetPos(_Pos);			// 位置
		CManager::SendPos(_Pos,PlayerID);
		CManager::SendRot(_Rot.y,PlayerID);

		CManager::SendCannonRot(Barrel->Rot(),PlayerID);


		if (_Hegiht > kHeightMax)
		{
			SetRespawn();
		}
		UpdateNari();
		return;
	}
	case PLAYER_STATE_RESPAWN:
	{
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;
		AddPosY(-kUpSpeed);
		_Hegiht += kUpSpeed;
  VECTOR3 pos = _Pos;
  pos.y += 20;
  pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
  pos.z += cosf(DEG2RAD(_Rot.y)) * -15;

  _nari->SetPos(pos);

		if (PlayerID == CManager::netData.charNum)
		{
			_Feed->AddAlpha(-Alpha);
		}
		_nari->AddAlpha(-Alpha * 3);
		Barrel->SetPos(_Pos);			// 位置
		CManager::SendPos(_Pos,PlayerID);
		CManager::SendRot(_Rot.y,PlayerID);
		CManager::SendCannonRot(Barrel->Rot(),PlayerID);

		if (_Hegiht > kHeightMax)
		{
			_State = PLAYER_STATE_WAIT;
			_Feed->SetAlpha(0);
			_nari->SetAlpha(0.0f);

			if(PlayerFlag)
			{
				Ballistic->SetDrawFlag(true);
			}

			CManager::SendReborn(PlayerID);
		}
		UpdateNari();
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
	// 仮想コントローラー
	VC* vc = VC::Instance();
	
	// 弾の使用状態を確定
	if (_Bullet == nullptr)
	{
		_BulletUseFlag = false;
	}

	// 初期化
	VECTOR3 rot = VECTOR3(0.0f,0.0f,0.0f);

	if (_InputFlag == true)
	{
		// 移動
		// 上

		Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed * vc->LeftStick().y;
		Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed * vc->LeftStick().y;
		
		if (vc->LeftStick().y > 0 || vc->LeftStick().y < 0)
		{
			IdlingSE->SetVolume(0);
			DriveSE->SetVolume(0.5f);
		}
		else
		{
			IdlingSE->SetVolume(0.5f);
			DriveSE->SetVolume(0);
		}

		//旋回
		rot.y -= PLAYER_ROTY_SPEED*vc->LeftStick().x;

		// 砲身の上下
		BarrelRotX -= BARREL_ROTX_SPEED * vc->RightStick().y;

		// ゲームが終了していない時
		if(_EndGameFlag == false)
		{
			// 弾の発射
			// 弾が発射されていなかった時
			if (_LaunchFlag == false)
			{
				if (vc->Trigger(COMMAND_SHOT))
				{
					_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),_PlayerColor);
					vc->SetVibration(0.5f,20,0.5f,20);
					CSoundAL::Play(CSoundAL::SE_CANNON,_Pos);
					_LaunchFlag = true;
					_BulletUseFlag = true;
					_ReloadTimer = 0;
					CManager::SendCannon(_LaunchFlag,PlayerID);
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
					_LaunchFlag = false;
					_BulletUseFlag = false;
					_ReloadTimer = PLAYER_RELOAD_TIME;
				}
			}
		}
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

	// 移動エフェクト
	if (_SandTime >= 0)
	{
		_SandTime--;
	}
	else if (abs(Movement.x) > 0.1f || abs(Movement.z) > 0.1f)
	{
		VECTOR3	posEffect = _Pos;
		VECTOR3 vecMove = VECTOR3(0.0f,0.0f,0.0f);

		if (Movement.x != 0)
		{
			vecMove.x = Movement.x / abs(Movement.x);
		}

		if (Movement.z != 0)
		{
			vecMove.z = Movement.z / abs(Movement.z);
		}

		posEffect.x -= 8.0f * vecMove.x;
		posEffect.z -= 8.0f * vecMove.z;
		posEffect.y -= 10.0f;

		CSandCloud::Create(posEffect);
		_SandTime = 16;
	}

	// 減速
	Movement *= 0.95f;

	// 砲身の位置、回転を更新
	Barrel->SetPos(_Pos);			// 位置
	Barrel->SetRot(_Rot);			// 回転
	Barrel->AddRotX(BarrelRotX);	// 上で設定した回転量に砲身のX軸回転量を加算

	// 弾道の更新
	Ballistic->Update(_Pos,VECTOR3(BarrelRotX,_Rot.y,_Rot.z));

	CManager::SendPos(_Pos,PlayerID);
	/*
	#ifdef ROT_QUART
	CManager::SendRot(VectorAxisRotation.x, VectorAxisRotation.y, VectorAxisRotation.z, RotationAxis, _Rot.y);
	#endif
	#ifdef ROT_NORMAL
	CManager::SendRot(_Rot.x, _Rot.y, _Rot.z, RotationAxis, rot.y);
	#endif
	*/
	CManager::SendRot(_Rot.y,PlayerID);
	CManager::SendCannonRot(Barrel->Rot(),PlayerID);

#ifdef _DEBUG
	// デバッグ用
	// 連射
	CKeyboard* keyboard = CKeyboard::Instance();
	if (keyboard->GetPress(DIK_V))
	{
		CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
	}

	// ライフの減算
	if (keyboard->GetTrigger(DIK_L))
	{
		this->AddPlayerLife(-1);
		_State = PLAYER_STATE_DAMAGE;
	}

	// 弾の削除確認
	if (keyboard->GetTrigger(DIK_M))
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
		_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),_PlayerColor);
		CSoundAL::Play(CSoundAL::SE_CANNON,_Pos);
		_LaunchFlag = true;
		_BulletUseFlag = true;
		_ReloadTimer = 0;
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

	// 移動値
	Movement = _Pos - _OldPos;

	// 移動エフェクト
	if (_SandTime >= 0)
	{
		_SandTime--;
	}
	else if (abs(Movement.x) > 0.1f || abs(Movement.z) > 0.1f)
	{
		VECTOR3	posEffect = _Pos;
		VECTOR3 vecMove = VECTOR3(0.0f,0.0f,0.0f);

		if (Movement.x != 0)
		{
			vecMove.x = Movement.x / abs(Movement.x);
		}

		if (Movement.z != 0)
		{
			vecMove.z = Movement.z / abs(Movement.z);
		}

		posEffect.x -= 8.0f * vecMove.x;
		posEffect.z -= 8.0f * vecMove.z;
		posEffect.y -= 10.0f;

		CSandCloud::Create(posEffect);
		_SandTime = 16;
	}

	if (abs(Movement.x) > 0.1f || abs(Movement.z) > 0.1f)
	{
		IdlingSE->SetVolume(0);
		DriveSE->SetVolume(0.3f);
	}
	else
	{
		IdlingSE->SetVolume(0.3f);
		DriveSE->SetVolume(0);
	}

	// ゲームが終了していない時
	if(_EndGameFlag == false)
	{
		// 弾が発射されている時
		if (_LaunchFlag == true)
		{
			// リロード可能までの時間を加算
			_ReloadTimer++;

			// リロード制限時間を超えたら
			if (_ReloadTimer >= PLAYER_RELOAD_TIME)
			{
				// 再発射可能に
				_LaunchFlag = false;
				_BulletUseFlag = false;
				_ReloadTimer = PLAYER_RELOAD_TIME;
			}
		}
	}

	// 現在の座標を保存
	_OldPos = _Pos;
}
//------------------------------------------------------------------------------
// なりの更新処理
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::UpdateNari(void)
{
	if (_State == PLAYER_STATE_RESPAWN || _State == PLAYER_STATE_DEATH)
	{
		if (_nari->Size().y > 1024 / NARI_SCL || _nari->Size().y < (1024 / NARI_SCL) / 2)
		{
			_NariSclSpeed *= -1;
		}
		_nari->AddSizeY(_NariSclSpeed);
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
void CPlayer::SetDeath(VECTOR3 pos,int _charNum)
{
	if (_State != PLAYER_STATE_DEATH)
	{
		CSoundAL::Play(CSoundAL::SE_BREAK,_Pos);
		CSoundAL::Play(CSoundAL::SE_DEATH,_Pos);
		if (PlayerFlag)
		{
			VC::Instance()->SetVibration(1.0f,60,1.0f,180);
			Ballistic->SetDrawFlag(false);	// 弾道を非表示に
		}
		_Hegiht = 0;
		_State = PLAYER_STATE_DEATH;
		_PlayerRespown = pos;
		_nari->SetAlpha(0.01f);

		if (_charNum == CManager::netData.charNum)
		{
			_Feed->SetAlpha(0);
			VECTOR3 pos = _Pos;
			pos.y += 10;
			pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
			pos.z += cosf(DEG2RAD(_Rot.y)) * -15;
			_nari->SetPos(pos);
		}
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
	_ReloadTimer = PLAYER_RELOAD_TIME;

	if (PlayerID == CManager::netData.charNum)
	{
		_Feed->SetAlpha(1);
		_PlayerRespown.y += 10;

		VECTOR3 pos = _Pos;
		pos.y += 20;
		pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
		pos.z += cosf(DEG2RAD(_Rot.y)) * -15;

		_nari->SetPos(pos);

		Movement = VECTOR3(0,0,0);

		Barrel->SetPos(_Pos);			// 位置
		CManager::SendPos(_Pos,PlayerID);

		/*
		#ifdef ROT_QUART
		CManager::SendRot(VectorAxisRotation.x, VectorAxisRotation.y, VectorAxisRotation.z, RotationAxis, _Rot.y);
		#endif

		#ifdef ROT_NORMAL
		CManager::SendRot(_Rot.x, _Rot.y, _Rot.z, RotationAxis, rot.y);
		#endif
		*/
		CManager::SendRot(_Rot.y,PlayerID);


		CManager::SendCannonRot(Barrel->Rot(),PlayerID);
		CManager::SendCannon(_LaunchFlag,PlayerID);

		// フラグの初期化
		_BulletUseFlag = false;
		_LaunchFlag = false;
	}
}
//------------------------------------------------------------------------------
// 加算処理
//------------------------------------------------------------------------------
// 引数
// addVal	: ライフ
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
		{
			if (_PlayerLife > PLAYER_LIFE)
			{
				_PlayerLife = PLAYER_LIFE;
			}
		}
	}
}

//------------------------------------------------------------------------------
// EOF