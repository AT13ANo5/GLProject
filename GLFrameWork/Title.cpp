//kikushima takahiro

#include "Title.h"
#include "Polygon2D.h"
#include "Texture.h"
#include "Keyboard.h"
#include "ManagerGL.h"
#include "Model.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Camera.h"
#include "CPushStart.h"
#include "SoundAL.h"
#include <math.h>
#include "TitleDirection.h"
#include "CPlayer.h"

// static member
const float CTitle::RADIUS_SKY = 500.0f;   // 空の半径

CTitle::CTitle()
{
	PushEnter = nullptr;
	Player = nullptr;
	Ground = nullptr;
	Sky = nullptr;
	Camera = nullptr;
	TitleD = nullptr;
	CameraRotation = 0.0f;
}

CTitle::~CTitle()
{
}

void CTitle::Init(void)
{
	//岩、バレル削除、もとからあったPlayerをCModelからCPlayerに変更 (kikushima) 2/16

	//タイトルロゴ
	TitleD->Create(VECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.0f, 0), VECTOR2(750.0f, 375.0f));

	// pushenter
	PushEnter = CPushStart::Create(VECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.25f, 0), VECTOR2(400.0f, 64.0f));
	PushEnter->SetTex(CTexture::Texture(TEX_PUSH_ENTER));
	PushEnter->SetAlphaSpeed(0.015f);

	// Player
	Player = CPlayer::Create(CModel::YOUJO, VECTOR3(0.0f + 0 * 50.0f, 30.0f, 0.0f), 0);
	Player->SetTex(CTexture::Texture(TEX_YOUJO_WHITE));
	Player->SetRot(0.0f,180.0f,0.0f);
	Player->setBarrelTex(TEX_YOUJO_WHITE);

	// Ground
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,-100.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// Sky
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	Camera = CCamera::Camera(0);

	Camera->SetEye(VECTOR3(0.0f,80.0f,0.0f));
	CSoundAL::Play(CSoundAL::BGM_TITLE);
}

void CTitle::Uninit(void)
{
	SafeRelease(Player);
	SafeRelease(TitleD);
	CMeshGround::ReleaseAll();
}

void CTitle::Update(void)
{
	// playerの移動処理　コメントアウト (kikushima)
	// TODO カメラ動かしてみるテスト（masuda）
	const float cameraLength = 200.0f;
	CameraRotation += 0.001f;
	Camera->SetEye(VECTOR3(0.0f - sinf(CameraRotation) * cameraLength,80.0f,0.0f - cosf(CameraRotation) * cameraLength));

	/*if (CKeyboard::GetPress(DIK_W))
	{
		Player->AddPosZ(1.0f);
		Barrel->AddPosZ(1.0f);
	}
	else if (CKeyboard::GetPress(DIK_S))
	{
		Player->AddPosZ(-1.0f);
		Barrel->AddPosZ(-1.0f);
	}
	if (CKeyboard::GetPress(DIK_A))
	{
		Player->AddRotY(1.0f);
		Barrel->AddRotY(1.0f);
	}
	else if (CKeyboard::GetPress(DIK_D))
	{
		Player->AddRotY(-1.0f);
		Barrel->AddRotY(-1.0f);
	}
	if (CKeyboard::GetPress(DIK_Q))
	{
		Barrel->AddRotZ(-1.0f);

	}
	if (CKeyboard::GetPress(DIK_E))
	{
		Barrel->AddRotZ(1.0f);

	}*/

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CSoundAL::Play(CSoundAL::SE_ENTER);
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

