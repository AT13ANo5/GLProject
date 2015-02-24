//kikushima takahiro
#include <float.h>
#include <math.h>
#include "Title.h"
#include "Polygon2D.h"
#include "Texture.h"
#include "Input/VC.h"
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
const float CTitle::RADIUS_SKY = 2400.0f;   // 空の半径
const int kRockMax = 34;
namespace{
 const VECTOR3 CAMERA_EYE = VECTOR3(50.0f,15.0f,-400.0f);
 const VECTOR3 CAMERA_LOOKAT = VECTOR3(0.0f,20.0f,0.0f);
}
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
	TitleD->Create(VECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.0f, 0), VECTOR2(650.0f, 320.0f));

	// pushenter
	PushEnter = CPushStart::Create(VECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.15f, 0), VECTOR2(300.0f, 52.0f));
	PushEnter->SetTex(CTexture::Texture(TEX_PUSH_ENTER));
	PushEnter->SetAlphaSpeed(0.015f);

	// Player
	Player = new CPlayer*[PLAYER_MAX];
 float rot = PI * 2 / PLAYER_MAX;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
  if(i % 2 == 1)
  {
   Player[i] = CPlayer::Create(CModel::RINCHAN,VECTOR3(sinf(MoveRot) *70.0f + i * 25.0f,30.0f,cosf(MoveRot) * 70.0f + cosf(rot * i) * 30.0f),i);
  }
  else
  {
   Player[i] = CPlayer::Create(CModel::YOUJO,VECTOR3(sinf(MoveRot) * 70.0f + i * 25.0f,30.0f,cosf(MoveRot) * 70.0f + cosf(rot * i) * 30.0f),i);
  }
		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_RED + i));
		Player[i]->SetRot(0.0f, 0.0f, 0.0f);
		Player[i]->setBarrelTex(TEX_YOUJO_RED + i);
	}

	// Ground
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0,0.0f),VECTOR2(40.0f,40.0f),VECTOR2(0.0f,0.0f),4.0f);
	Ground->SetTex(CTexture::Texture(TEX_FIELD));
 // Rock
 float rock_rot = PI * 2 / kRockMax;
 Rock = new CModel*[kRockMax];
 for(int i = 0; i < kRockMax; i++)
 {
  if(i % 2 == 0)
  {
   Rock[i] = CModel::Create(CModel::ROCK,VECTOR3(sinf(rock_rot * i) * -1600.0f,30.0f,cosf(rock_rot * i) * -1600.0f));
   Rock[i]->SetRotX(RAD2DEG(-rock_rot * i));
   Rock[i]->SetRotY(RAD2DEG(rock_rot * i));
   Rock[i]->SetRotZ(RAD2DEG(-rock_rot * i));
  }
  else{
   Rock[i] = CModel::Create(CModel::ROCK,VECTOR3(sinf(rock_rot * i) * -800.0f,30.0f,cosf(rock_rot * i) * -800.0f));
   Rock[i]->SetScl(0.8f,0.8f,0.8f);
   Rock[i]->SetRotX(RAD2DEG(rock_rot * i));
   Rock[i]->SetRotY(RAD2DEG(-rock_rot * i));
   Rock[i]->SetRotZ(RAD2DEG(rock_rot * i));
  }
  Rock[i]->SetTex(CTexture::Texture(TEX_ROCK));
  Rock[i]->SetPosY(Ground->GetHeight(Rock[i]->Pos()) + 16.0f);

 }

	// Sky
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,50.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	Camera = nullptr;
 Camera = CCamera::Camera(0);
 Camera->SetLookat(CAMERA_LOOKAT);
 MoveSpeed = 2.0f;
 RotSpeed = -0.003f;
 MoveRot = 0.0f;
 CameraRotation = PI;
	CSoundAL::Play(CSoundAL::BGM_TITLE);
}

void CTitle::Uninit(void)
{
 for(int i = 0; i < kRockMax; i++)
 {
  SafeRelease(Rock[i]);
 }
 SafeDeletes(Rock);
  for(int i = 0; i < PLAYER_MAX; i++)
	{
		SafeRelease(Player[i]);
	}
 SafeDeletes(Player);
 SafeRelease(TitleD);
	SafeDelete(Camera);
 SafeRelease(Sky);
 SafeRelease(PushEnter);
 SafeRelease(Ground);
	CMeshGround::ReleaseAll();
 CObject::ReleaseAll();
}

void CTitle::Update(void)
{
 Sky->SetPosX(Player[4]->Pos().x);
 Sky->SetPosZ(Player[4]->Pos().z);
 MoveRot += RotSpeed;
 REVISE_PI(MoveRot);
 for(int i = 0; i < PLAYER_MAX; i++)
 {
  VECTOR3 PlayerPos = Player[i]->Pos();


  PlayerPos.x -= sinf(MoveRot) * MoveSpeed;
  PlayerPos.z -= cosf(MoveRot) * MoveSpeed;
  Player[i]->SetPosX(PlayerPos.x);
  Player[i]->SetPosZ(PlayerPos.z);

  Player[i]->SetRotY(180 + RAD2DEG(MoveRot));

  VECTOR3	NormalGround;		// 地形の法線

  float pos = Ground->GetHeight(Player[i]->Pos(),&NormalGround);


  //********************************************************
  // 2015_02_12 姿勢制御用の処理を追加 ここから
  //********************************************************
  // 回転を求める
  VECTOR3	vectorUp(0.0f,1.0f,0.0f);		// 上方向ベクトル
  VECTOR3	vectorAxisRotation;				// 回転軸
  float	rotation = 0.0f;				// 回転量
  VECTOR3::Cross(&vectorAxisRotation,NormalGround,vectorUp);
  if(vectorAxisRotation.x < FLT_EPSILON && vectorAxisRotation.x > -FLT_EPSILON)
  {
   if(vectorAxisRotation.z < FLT_EPSILON && vectorAxisRotation.z > -FLT_EPSILON)
   {
    if(vectorAxisRotation.y < FLT_EPSILON && vectorAxisRotation.y > -FLT_EPSILON)
    {
     vectorAxisRotation.y = 1.0f;
    }
   }
  }
  vectorAxisRotation.Normalize();
  rotation = VECTOR3::Dot(NormalGround,vectorUp);
  if(rotation <= 1.0f && rotation >= -1.0f)
  {
   rotation = RAD_TO_DEG * acosf(rotation);
  }
  else
  {
   rotation = 0.0f;
  }
  Player[i]->SetPosY(pos + 12.0f);

  // キャラクターに設定する
  Player[i]->SetAxisRotation(vectorAxisRotation);
  Player[i]->SetRotationAxis(rotation);
  //********************************************************
  // 2015_02_12 姿勢制御用の処理を追加 ここまで
  //********************************************************
  Player[i]->setBarrelRot(Player[i]->Rot());
 }

 // TODO カメラ回す
 const float cameraLength = 120.0f;
 CameraRotation += 0.004f;
 REVISE_PI(CameraRotation);
 float pos = Ground->GetHeight(Camera->Eye());
 Camera->SetEye(VECTOR3(Player[3]->Pos().x + sinf(CameraRotation) * cameraLength,pos + 40.0f,Player[3]->Pos().z + cosf(CameraRotation) * cameraLength));
 Camera->SetLookat(Player[3]->Pos());

	if (VC::Instance()->Trigger(COMMAND_OK))
	{
		CSoundAL::Play(CSoundAL::SE_ENTER);
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

