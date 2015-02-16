//------------------------------------------------------------------------------
//
//  []
// Author : 
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------------
#include <math.h>
#include <float.h>

#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Explosion.h"
#include "Spark.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "BattleAreaCylinder.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "PlayerCamera.h"
#include "MiniMap.h"
#include "SoundAL.h"

#include "UI.h"

// 静的メンバ変数
const float CGame::RADIUS_SKY = 2500.0f;   // 空の半径
CPlayer** CGame::Player;
CPlayer* Player = nullptr;//プレイヤー
const float	CGame::RADIUS_DEFENSE_CHARACTER = 12.0f;	// キャラクターの防御半径
const float	CGame::HEIGHT_DEFENSE_CHARACTER = 0.0f;		// キャラクターの防御中心高さ
const float	CGame::RADIUS_OFFENSE_BULLET = 10.0f;		// 砲弾の攻撃半径
const float	CGame::HEIGHT_OFFENSE_BULLET = 0.0f;		// 砲弾の攻撃中心高さ
const float	CGame::RADIUS_PUSH_CHARACTER = 10.0f;		// キャラクターの押し戻し半径
const float	CGame::HEIGHT_PUSH_CHARACTER = 0.0f;		// キャラクターの押し戻し中心高さ
const float	CGame::RADIUS_DEFENSE_ROCK = 28.0f;			// 岩の防御半径
const float	CGame::HEIGHT_DEFENSE_ROCK = 0.0f;			// 岩の防御中心高さ
const float	CGame::RADIUS_PUSH_ROCK = 38.0f;			// 岩の押し戻し半径
const float	CGame::HEIGHT_PUSH_ROCK = 0.0f;				// 岩の押し戻し中心高さ
const float CGame::FIELD_PANEL_SIZE = 35.0f;			//フィールドのパネル一枚のサイズ
const float	CGame::HEIGHT_PLAYER_TO_FIELD = 10.0f;		// プレイヤーと地面の差

const float	CGame::RADIUS_AREA_BATTLE = 1000.0f;		// 戦闘エリア半径
const float	CGame::HEIGHT_WALL = 500.0f;				// 壁の高さ

const int	CGame::MAX_ROCK = 20;						// 岩の数

const VECTOR3 CGame::ROCK_POSITION_LIST[] = {
	VECTOR3(-214.0f,100.0f,421.0f),
	VECTOR3(359.0f,100.0f,188.0f),
	VECTOR3(94.0f,100.0f,-458.0f),
	VECTOR3(-198.0f,100.0f,222.0f),
	VECTOR3(419.0f,100.0f,293.0f),
	VECTOR3(-335.0f,100.0f,164.0f),
	VECTOR3(-471.0f,100.0f,-115.0f),
	VECTOR3(368.0f,100.0f,-363.0f),
	VECTOR3(-476.0f,100.0f,231.0f),
	VECTOR3(-249.0f,100.0f,-319.0f),
	VECTOR3(-243.0f,100.0f,481.0f),
	VECTOR3(345.0f,100.0f,-253.0f),
	VECTOR3(444.0f,100.0f,-118.0f),
	VECTOR3(186.0f,100.0f,27.0f),
	VECTOR3(387.0f,100.0f,-438.0f),
	VECTOR3(-12.0f,100.0f,-439.0f),
	VECTOR3(496.0f,100.0f,-332.0f),
	VECTOR3(-247.0f,100.0f,143.0f),
	VECTOR3(-302.0f,100.0f,-296.0f),
	VECTOR3(-171.0f,100.0f,-274.0f),

};

// 定数
namespace{

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
CGame::CGame()
{

	UI = nullptr;
	ppRock_ = nullptr;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
CGame::~CGame()
{

}

//------------------------------------------------------------------------------
// ゲームの初期化
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CGame::Init(void)
{
	CSoundAL::Play(CSoundAL::BGM_GAME);
	//地形生成
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(FIELD_PANEL_SIZE,FIELD_PANEL_SIZE),VECTOR2(0,0),1.5f);
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// 空生成
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	// 境界線生成
	CylinderArea = nullptr;
	CylinderArea = CBattleAreaCylinder::Create(VECTOR3(0.0f,0.0f,0.0f),HEIGHT_WALL,VECTOR2(64.0f,1.0f),RADIUS_AREA_BATTLE,VECTOR2(1,-0.5f));
	CylinderArea->SetTex(CTexture::Texture(TEX_WALL));
	CylinderArea->SetAlpha(0.5f);

#ifdef _DEBUG
	// デバッグワイヤーフレーム
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	Player = new CPlayer*[PLAYER_MAX];
	// プレイヤー生成
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (i % 2 == 1)
			Player[i] = CPlayer::Create(CModel::RINCHAN,VECTOR3(0.0f + i * 50.0f,30.0f,0.0f),i);
		else
			Player[i] = CPlayer::Create(CModel::YOUJO, VECTOR3(0.0f + i * 50.0f, 30.0f, 0.0f), i);

		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_RED + i));
		Player[i]->SetRot(0.0f,180.0f,0.0f);
		Player[i]->setBarrelTex(TEX_YOUJO_RED + i);

		if (i == CManager::netData.charNum)
		{
			Player[i]->SetPlayerFlag(true);
		}
	}	//プレイヤーカメラ生成
	CPlayerCamera::Create(Player[CManager::netData.charNum],35.0f);

	// UI初期化
	UI = new CUI;
	UI->Init();
	UI->setMyID(CManager::netData.charNum);
	UI->SetPlayer(Player);
	UI->MiniMap()->SetFieldSize(VECTOR3(RADIUS_AREA_BATTLE*2.25f,0,RADIUS_AREA_BATTLE*2.25f));

	// 岩の生成
	ppRock_ = new CModel*[MAX_ROCK];

	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		ppRock_[cntRock] = CModel::Create(CModel::ROCK,ROCK_POSITION_LIST[cntRock]);
		ppRock_[cntRock]->SetScl(1,1,1);
		ppRock_[cntRock]->SetTex(CTexture::Texture(TEX_ROCK));
		PushBackObjectByField(ppRock_[cntRock]);
	}

	CManager::gameStartFlag = true;

	CManager::sendGameStart();
}

//=============================================================================
//	プレイヤー情報のセット関数
//=============================================================================
void CGame::SetPlayerState(NET_DATA _netData,DATA_TYPE _dataType)
{
	//	自分以外のデータなら
	if (_netData.charNum != CManager::netData.charNum)
	{
		switch (_dataType)
		{
		case DATA_TYPE_POS:

			Player[_netData.charNum]->SetPosX(_netData.data_pos.posX);
			Player[_netData.charNum]->SetPosY(_netData.data_pos.posY);
			Player[_netData.charNum]->SetPosZ(_netData.data_pos.posZ);

			break;

		case DATA_TYPE_ROT:

			Player[_netData.charNum]->SetRotX(_netData.data_rot.rotX);
			Player[_netData.charNum]->SetRotY(_netData.data_rot.rotY);
			Player[_netData.charNum]->SetRotZ(_netData.data_rot.rotZ);

			break;

		case DATA_TYPE_CANNONROT:

			Player[_netData.charNum]->setBarrelRot(
				VECTOR3(_netData.data_cannonRot.rotX,
				_netData.data_cannonRot.rotY,
				_netData.data_cannonRot.rotZ
				));

			break;

		case DATA_TYPE_CANNON:

			if (_netData.data_cannon.flag == true)
			{
				Player[_netData.charNum]->BlastBullet();
			}

			break;

		case DATA_TYPE_DEATH:

			break;

		case DATA_TYPE_KILL:

			break;
		}
	}
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	
	CManager::SendKillDeath(Player[CManager::netData.charNum]->getKillCount(),
							Player[CManager::netData.charNum]->getDeathCount());

	CManager::gameStartFlag = false;

	// 岩の破棄
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		SafeRelease(ppRock_[cntRock]);
		ppRock_[cntRock] = nullptr;
	}
	SafeDeletes(ppRock_);

	// プレイヤー破棄
	//SafeRelease(Player);
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		SafeRelease(Player[i]);
	}

	SafeDeletes(Player);

	// 境界線の破棄
	SafeRelease(CylinderArea);

	// 空破棄
	if (Sky != nullptr)
	{
		Sky->Release();
		Sky = nullptr;
	}

	// 地形破棄
	if (Ground != nullptr)
	{
		Ground->Release();
		Ground = nullptr;
	}

	// UI破棄
	UI->Uninit();
	SafeDelete(UI);

	CCamera::ReleaseAll();
	CObject::ReleaseAll();
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CGame::Update(void)
{
	// 装填ゲージ
	const float currentTimer = (float)Player[0]->ReloadTimer();
	const float maxTimer = (float)PLAYER_RELOAD_TIME;
	const float rate = currentTimer / maxTimer;


	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		if (CManager::netData.charNum == 0)
		{
			CManager::SendChangeResult();
			CManager::ChangeScene(SCENE_RESULT);
		}
	}

	// 空の位置プレイヤーに合わせる
	Sky->SetPosX(Player[0]->Pos().x);
	Sky->SetPosZ(Player[0]->Pos().z);
	for (int loop = 0;loop < PLAYER_MAX;loop++)
	{
		if (Player[loop]->PlayerLife() <= 0)
		{
			VECTOR3	NormalGround;		// 地形の法線
			VECTOR3 Respawn = VECTOR3(rand() % 100 + 0.0f,0,rand() % 100 + 0.0f);
			Respawn.y = Ground->GetHeight(Respawn,&NormalGround);

			Player[loop]->SetDeath(Respawn, loop);
		}
	}
	// 攻撃判定
	CheckHitPlayer();

	// 攻撃判定
	CheckHitRock();

	// キャラクター同士の押し戻し
	PushBackCharacter();

	// キャラクターと岩の押し戻し
	PushBackRock();

	// 地形との押し戻し
	PushBackField();

	// 地形との判定
	IsLandField();

	// 行動可能範囲判定
	PushBackBattleArea();

	// UIのアップデート
	UI->Update();

}

//==============================================================================
// 攻撃判定の当たり判定 プレイヤー
//==============================================================================
void CGame::CheckHitPlayer(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player[cntBullet];

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
		{
			// 自身とは判定しない
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// プレイヤーを取得
			CPlayer*	pPlayerDefense = Player[cntPlayer];		// 防御側プレイヤー

			// プレイヤーが判定可能か確認
			if (NeedsSkipPlayer(pPlayerDefense))
			{
				continue;
			}

			// 当たり判定
			VECTOR3	positionOffense = pPlayerOffense->Bullet()->Pos();		// 攻撃判定中心座標
			VECTOR3	positionDefense = pPlayerDefense->Pos();				// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;									// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;								// 判定の中心同士の距離

			positionOffense.y += HEIGHT_OFFENSE_BULLET;
			positionDefense.y += HEIGHT_DEFENSE_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z;

			if (distanceOffenseAndDefense < (RADIUS_DEFENSE_CHARACTER + RADIUS_OFFENSE_BULLET) * (RADIUS_DEFENSE_CHARACTER + RADIUS_OFFENSE_BULLET))
			{
				// 弾の消滅
				pPlayerOffense->ReleaseBullet();

				// 当たったときの処理
				pPlayerDefense->AddPlayerLife(-1);
				pPlayerDefense->SetState(PLAYER_STATE_DAMAGE);

				// エフェクト：爆発　弾がプレイヤーに当たったとき
				CExplosion::Create(pPlayerDefense->Pos());

				//	長崎
				if (pPlayerDefense->PlayerLife() == 0)
				{
					//	殺された数加算
					pPlayerDefense->addDeathCount();
					CManager::SendDeath(pPlayerDefense->getDeathCount(), pPlayerDefense->getPlayerID());

					//	殺した数加算
					pPlayerOffense->addKillCount();
					CManager::SendKill(pPlayerOffense->getKillCount(), pPlayerOffense->getPlayerID());
				}

				// 処理終了
				break;
			}
		}
	}
}

//==============================================================================
// 攻撃判定の当たり判定 岩
//==============================================================================
void CGame::CheckHitRock(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player[cntBullet];

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// プレイヤーを取得
			CObject*	pRock = ppRock_[cntRock];		// 防御側岩

			// 当たり判定
			VECTOR3	positionOffense = pPlayerOffense->Bullet()->Pos();		// 攻撃判定中心座標
			VECTOR3	positionDefense = pRock->Pos();							// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;									// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;								// 判定の中心同士の距離
			float	scalingRock;											// 岩の大きさ
			scalingRock = (pRock->Scl().x < pRock->Scl().z ? pRock->Scl().x : pRock->Scl().z);
			positionOffense.y += HEIGHT_OFFENSE_BULLET;
			positionDefense.y += HEIGHT_DEFENSE_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z;

			if (distanceOffenseAndDefense < (RADIUS_DEFENSE_ROCK * scalingRock + RADIUS_OFFENSE_BULLET) * (RADIUS_DEFENSE_ROCK * scalingRock + RADIUS_OFFENSE_BULLET))
			{
				// 弾の消滅
				pPlayerOffense->ReleaseBullet();

				// エフェクト：爆発　弾が岩に当たったとき
				CExplosion::Create(positionOffense);

				// 処理終了
				break;
			}
		}
	}
}

//==============================================================================
// キャラクター同士の押し戻し
//==============================================================================
void CGame::PushBackCharacter(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player[cntBullet];

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
		{
			// プレイヤーを取得
			CPlayer*	pPlayerDefense = Player[cntPlayer];		// 防御側プレイヤー

			// プレイヤーが判定可能か確認
			if (NeedsSkipPlayer(pPlayerDefense))
			{
				continue;
			}
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// 当たり判定
			VECTOR3	positionOffense = pPlayerOffense->Pos();		// 攻撃判定中心座標
			VECTOR3	positionDefense = pPlayerDefense->Pos();		// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;							// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;						// 判定の中心同士の距離
			positionOffense.y += HEIGHT_PUSH_CHARACTER;
			positionDefense.y += HEIGHT_PUSH_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);
			if (distanceOffenseAndDefense < 2.0f * RADIUS_PUSH_CHARACTER)
			{
				// 押し戻し
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * 0.51f * (2.0f * RADIUS_PUSH_CHARACTER - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					pPlayerDefense->AddPos(vectorPushBack);
					vectorPushBack *= -1.0f;
					pPlayerOffense->AddPos(vectorPushBack);
				}
				else
				{
					pPlayerOffense->AddPosX(RADIUS_PUSH_CHARACTER);
					pPlayerDefense->AddPosX(-RADIUS_PUSH_CHARACTER);
				}

				// エフェクト：火花　プレイヤー同士のぶつかり
			}
		}
	}
}

//==============================================================================
// キャラクターと岩の押し戻し
//==============================================================================
void CGame::PushBackRock(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayer = nullptr;		// 攻撃側プレイヤー
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// プレイヤーを取得
		pPlayer = Player[cntPlayer];

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(pPlayer))
		{
			continue;
		}

		// 防御側の決定
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// 岩を取得
			CObject*	pRock = ppRock_[cntRock];		// 岩オブジェクト

			// 当たり判定
			VECTOR3	positionPlayer = pPlayer->Pos();	// 攻撃判定中心座標
			VECTOR3	positionRock = pRock->Pos();		// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;				// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;			// 判定の中心同士の距離
			float	scalingRock;						// 岩の大きさ
			scalingRock = (pRock->Scl().x < pRock->Scl().z ? pRock->Scl().x : pRock->Scl().z);
			positionPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positionPlayer;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);

			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock)
			{
				// 押し戻し
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * -(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					vectorPushBack.y = 0.0f;
					pPlayer->AddPos(vectorPushBack);
				}
				else
				{
					pPlayer->AddPosX(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock);
				}

				// エフェクト：火花　プレイヤーと岩のぶつかり
			}
		}
	}
}

//==============================================================================
// 地形の押し戻し
//==============================================================================
void CGame::PushBackField(void)
{
	// 判定
	CPlayer*	pPlayerCurrent = nullptr;		// 対象オブジェクト
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		pPlayerCurrent = Player[cntPlayer];

		// 対象のステートを確認
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// 押し戻し
		PushBackObjectByField(pPlayerCurrent);
	}
}

//==============================================================================
// 地形との判定
//==============================================================================
void CGame::IsLandField(void)
{
	// 判定
	CPlayer*	pPlayerCurrent = nullptr;		// 対象プレイヤー
	CBullet*	pBulletCurrent = nullptr;		// 対象オブジェクト
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// 対象プレイヤーの取得
		pPlayerCurrent = Player[cntBullet];

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerCurrent))
		{
			continue;
		}

		// 対象オブジェクトを取得
		pBulletCurrent = pPlayerCurrent->Bullet();

		// 判定
		CBullet	bulletHit = *pBulletCurrent;
		PushBackObjectByField(&bulletHit);
		if (bulletHit.Pos().y >= pBulletCurrent->Pos().y)
		{
			// 弾の消滅処理
			pPlayerCurrent->ReleaseBullet();

			// エフェクト：爆発　弾と地形の判定
			CExplosion::Create(pBulletCurrent->Pos());
		}
	}
}

//==============================================================================
// オブジェクトの地形による押し戻し
//==============================================================================
void CGame::PushBackObjectByField(CObject* pObject)
{
	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	float	HeightGround;		// 地形の高さ
	HeightGround = Ground->GetHeight(pObject->Pos(), &NormalGround) + HEIGHT_PLAYER_TO_FIELD;

	//********************************************************
	// 2015_02_12 姿勢制御用の処理を追加 ここから
	//********************************************************
	// 回転を求める
	VECTOR3	vectorUp(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	VECTOR3	vectorAxisRotation;				// 回転軸
	float	rotation = 0.0f;				// 回転量
	VECTOR3::Cross(&vectorAxisRotation, NormalGround, vectorUp);
	if (vectorAxisRotation.x < FLT_EPSILON && vectorAxisRotation.x > -FLT_EPSILON)
	{
		if (vectorAxisRotation.z < FLT_EPSILON && vectorAxisRotation.z > -FLT_EPSILON)
		{
			if (vectorAxisRotation.y < FLT_EPSILON && vectorAxisRotation.y > -FLT_EPSILON)
			{
				vectorAxisRotation.y = 1.0f;
			}
		}
	}
	vectorAxisRotation.Normalize();
	rotation = VECTOR3::Dot(NormalGround, vectorUp);
	if (rotation > 2.0f * FLT_EPSILON || rotation < -2.0f * FLT_EPSILON)
	{
		rotation = RAD_TO_DEG * acosf(rotation);
	}
	else
	{
		rotation = 0.0f;
	}

	// キャラクターに設定する
	pObject->SetPosY(HeightGround);
	pObject->SetAxisRotation(vectorAxisRotation);
	pObject->SetRotationAxis(rotation);
	//********************************************************
	// 2015_02_12 姿勢制御用の処理を追加 ここまで
	//********************************************************
}

//==============================================================================
// プレイヤー判定スキップ
//==============================================================================
void CGame::PushBackBattleArea(void)
{
	// 判定
	CPlayer*	pPlayerCurrent = nullptr;		// 対象オブジェクト
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		pPlayerCurrent = Player[cntPlayer];

		// 対象のステートを確認
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// 押し戻し
		VECTOR3	vectorPlayerToCenter = Ground->Pos() - pPlayerCurrent->Pos();
		vectorPlayerToCenter.y = 0.0f;
		float	distanceFromCenter = vectorPlayerToCenter.x * vectorPlayerToCenter.x + vectorPlayerToCenter.y * vectorPlayerToCenter.y + vectorPlayerToCenter.z * vectorPlayerToCenter.z;
		if (distanceFromCenter > RADIUS_AREA_BATTLE * RADIUS_AREA_BATTLE)
		{
			float	distancePushBack = sqrtf(distanceFromCenter) - RADIUS_AREA_BATTLE;
			vectorPlayerToCenter.Normalize();
			pPlayerCurrent->AddPos(vectorPlayerToCenter * distancePushBack);
		}
	}
}

//==============================================================================
// プレイヤー判定スキップ
//==============================================================================
bool CGame::NeedsSkipPlayer(CPlayer* pPlayer)
{
	// エラーチェック
	if (pPlayer == nullptr)
	{
		return true;
	}

	// ステートを確認
	int		statePlayer = pPlayer->State();
	if (statePlayer == PLAYER_STATE_DEATH || statePlayer == PLAYER_STATE_RESPAWN)
	{
		return true;
	}

	// スキップしない
	return false;
}

//==============================================================================
// 砲弾判定スキップ
//==============================================================================
bool CGame::NeedsSkipBullet(CPlayer* pPlayer)
{
	// 弾が存在しないとき
	if (!pPlayer->BulletUseFlag())
	{
		return true;
	}

	// スキップしない
	return false;
}

//------------------------------------------------------------------------------
// EOF
