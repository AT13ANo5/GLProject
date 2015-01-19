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
#include <math.h>

// static member
const float CTitle::RADIUS_SKY = 500.0f;   // 空の半径

CTitle::CTitle()
{
	Logo = nullptr;
	PushEnter = nullptr;
	Player = nullptr;
	Ground = nullptr;
	Sky = nullptr;
	Camera = nullptr;
	CameraRotation = 0.0f;
}

CTitle::~CTitle()
{
}

void CTitle::Init(void)
{
	// Logo
	Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 3.0f,0),VECTOR2(750.0f,375.0f));
	Logo->SetTex(CTexture::Texture(TEX_TITLELOGO));

	// pushenter
	PushEnter = CPushStart::Create(VECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.25f, 0), VECTOR2(400.0f, 64.0f));
	PushEnter->SetTex(CTexture::Texture(TEX_PUSH_ENTER));
	PushEnter->SetAlphaSpeed(0.015f);

	// Player
	Player = CModel::Create(CModel::RINCHAN,VECTOR3(0.0f,0.0f,0.0f));
	Player->SetTex(CTexture::Texture(TEX_YOUJO_WHITE));
	Player->SetRot(0.0f,180.0f,0.0f);

	//Barrel
	Barrel = CModel::Create(CModel::TANK_BARREL,VECTOR3(0.0f,0.0f,0.0f));
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_WHITE));
	Barrel->SetRot(0.0f,180.0f,0.0f);

	// Ground
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,-100.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	//Rock
	Rock = CModel::Create(CModel::ROCK,VECTOR3(0.0f,0.0f,0.0f));
	Rock->SetTex(CTexture::Texture(TEX_ROCK));

	// Sky
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	Camera = CCamera::Camera();

	Camera->SetEye(VECTOR3(0.0f,80.0f,0.0f));
}

void CTitle::Uninit(void)
{
	SafeRelease(Rock);
	SafeRelease(Logo);
	SafeRelease(Player);
	SafeRelease(Barrel);
	CMeshGround::ReleaseAll();
}

void CTitle::Update(void)
{
	// TODO カメラ動かしてみるテスト（masuda）
	const float cameraLength = 200.0f;
	CameraRotation += 0.001f;
	Camera->SetEye(VECTOR3(0.0f - sinf(CameraRotation) * cameraLength,80.0f,0.0f - cosf(CameraRotation) * cameraLength));

	if (CKeyboard::GetPress(DIK_W))
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

	}
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

