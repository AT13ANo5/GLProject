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
	Player = new CPlayer*[PLAYER_MAX];
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		Player[i] = CPlayer::Create(CModel::YOUJO, VECTOR3(-100.0f + i * 40.0f, 30.0f, -100.0f), 0);
		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_RED + i));
		Player[i]->SetRot(0.0f, 180.0f, 0.0f);
		Player[i]->setBarrelTex(TEX_YOUJO_RED + i);
	}

	// Ground
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,-100.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// Sky
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	Camera = nullptr;
	Camera = CCamera::Camera(0);
	Camera->SetLookat(VECTOR3(0.0f, 0.0f, 0.0f));

	CSoundAL::Play(CSoundAL::BGM_TITLE);
}

void CTitle::Uninit(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		SafeRelease(Player[i]);
	}
	SafeRelease(TitleD);
	SafeDelete(Camera);
	SafeDelete(Sky);
	SafeDelete(PushEnter);
	CMeshGround::ReleaseAll();
}

void CTitle::Update(void)
{

	// TODO カメラ動かしてみるテスト（masuda）
	const float cameraLength = 200.0f;
	Camera->SetEye(VECTOR3(0.0f - sinf(CameraRotation) * cameraLength,80.0f,0.0f - cosf(CameraRotation) * cameraLength));

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CSoundAL::Play(CSoundAL::SE_ENTER);
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

