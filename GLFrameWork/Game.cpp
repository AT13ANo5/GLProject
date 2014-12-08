#include <math.h>

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
#include "Camera.h"
#include "PlayerCamera.h"

// 静的メンバ変数
const float CGame::RADIUS_SKY = 500.0f;   // 空の半径
#include "CPlayer.h"

CPlayer* g_player;
CModel* g_model;


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
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(100.0f, 100.0f), VECTOR2(2.0f, 2.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// 空生成
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

#ifdef _DEBUG
	// プレイヤー生成
	PlayerTest = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	PlayerTest->SetTex(CTexture::Texture(TEX_MIKU));
	PlayerTest->SetScl(20.0f, 20.0f, 20.0f);

	// デバッグワイヤーフレーム
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// デバッグカメラ位置
//	CCamera::Camera()->SetEye(0.0f, 100.0f, 0.0f);
#endif
}

void CGame::Uninit(void)
{
#ifdef _DEBUG
	// プレイヤー破棄
	if (PlayerTest != nullptr)
	{
		PlayerTest->Release();
		PlayerTest = nullptr;
	}
#endif

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
#ifdef _DEBUG
	// プレイヤー移動
	if (CKeyboard::GetPress(DIK_W))
	{
		PlayerTest->AddPosZ(1.0f);
	}
	else if (CKeyboard::GetPress(DIK_S))
	{
		PlayerTest->AddPosZ(-1.0f);
	}
	if (CKeyboard::GetPress(DIK_A))
	{
		PlayerTest->AddPosX(1.0f);
	}
	else if (CKeyboard::GetPress(DIK_D))
	{
		PlayerTest->AddPosX(-1.0f);
	}

	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	float	HeightGround;		// 地形の高さ
	HeightGround = Ground->GetHeight(PlayerTest->Pos(), &NormalGround);

	// 回転を求める
	VECTOR3	VectorUpPlayer;		// 上方向ベクトル
	VECTOR3	VectorNormalYZ;		// YZ平面上の法線ベクトル
	VECTOR3	VectorNormalXY;		// XY平面上の法線ベクトル
	float	AnglePlayerX;		// プレイヤー回転X軸
	float	AnglePlayerZ;		// プレイヤー回転Z軸
	VectorUpPlayer.x = VectorUpPlayer.z = 0.0f;
	VectorUpPlayer.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglePlayerX = acosf(VECTOR3::Dot(VectorNormalYZ, VectorUpPlayer));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = acosf(VECTOR3::Dot(VectorNormalXY, VectorUpPlayer));

	// プレイヤーに設定する
	PlayerTest->SetPosY(HeightGround);
	PlayerTest->SetRotX(180.0f / PI * AnglePlayerX);
	PlayerTest->SetRotZ( 180.0f / PI * AnglePlayerZ);
#endif

	// シーン遷移
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}