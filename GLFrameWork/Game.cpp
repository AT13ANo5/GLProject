#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"

// 静的メンバ変数
const float CGame::RADIUS_SKY = 500.0f;   // 空の半径
#include "CPlayer.h"

CGame::CGame()
{

}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	// 地形生成
	Ground = nullptr;
	/*
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));
	*/

	// 空生成
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_MIKU));

	// プレイヤー生成
	Player = CPlayer::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	Player->SetTex(CTexture::Texture(TEX_MIKU));
	Player->SetScl(20.0f, 20.0f, 20.0f);
}

void CGame::Uninit(void)
{
  // プレイヤー破棄
  if (Player != nullptr)
  {
    Player->Release();
    Player = nullptr;
  }

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

	CObject::ReleaseAll();
}

void CGame::Update(void)
{
  // TODO：以下一行コメントアウトは作業しにくいから。終了し次第消します
  // 地形とのあたり判定
  VECTOR3 NormalGround;   // 地形の法線
  //float   HeightGround;   // 地形の高さ
  //HeightGround = Ground->GetHeight(Player->Pos(), &NormalGround);
  NormalGround.Normalize();

  // 回転を求める
  VECTOR3 VectorUpPlayer; // 上方向ベクトル
  VECTOR3 VectorNormalYZ; // YZ平面上の法線ベクトル
  VECTOR3 VectorNormalXY; // XY平面上の法線ベクトル
  float   AnglePlayerX;   // プレイヤー回転X軸
  float   AnglePlayerZ;   // プレイヤー回転Z軸
  VectorUpPlayer.x = VectorUpPlayer.z = 0.0f;
  VectorUpPlayer.y = 1.0f;
  VectorNormalYZ.x = 0.0f;
  VectorNormalYZ.y = NormalGround.y;
  VectorNormalYZ.z = NormalGround.z;
  VectorNormalYZ.Normalize();
  AnglePlayerX = VECTOR3::Dot(VectorNormalYZ, VectorUpPlayer);
  VectorNormalXY.x = NormalGround.x;
  VectorNormalXY.y = NormalGround.y;
  VectorNormalXY.z = 0.0f;
  VectorNormalXY.Normalize();
  AnglePlayerZ = VECTOR3::Dot(VectorNormalXY, VectorUpPlayer);

  // プレイヤーに設定する
  //Player->SetPosY(HeightGround);
  //Player->SetRot(180.0f / PI * AnglePlayerX, 0.0f, 180.0f / PI * AnglePlayerZ);

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}