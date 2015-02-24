//=============================================================================
//
// Result [Result.cpp]
//
// update : 2015/02/11 masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Result.h"
#include "ManagerGL.h"
#include "Input/VC.h"
#include "Polygon2D.h"
#include "Model.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "ResultUIManager.h"
#include "Fade.h"
#include "Confetti.h"
#include "SoundAL.h"
#include "Camera.h"

//=============================================================================
// static menber
//=============================================================================
// ê√ìIÉÅÉìÉoïœêî
const float CResult::RADIUS_SKY = 1000.0f;		// ãÛÇÃîºåa

//=============================================================================
// macro
//=============================================================================
namespace{
	const VECTOR3 CAMERA_EYE = VECTOR3(50.0f,15.0f,-400.0f);
	const VECTOR3 CAMERA_LOOKAT = VECTOR3(0.0f,20.0f,0.0f);
}
//=============================================================================
// constructor
//=============================================================================
CResult::CResult()
{
	Ground = nullptr;
	Sky = nullptr;
	for (int loop = 0;loop < PLAYER_MAX;loop++)
	{
		Player[loop] = nullptr;
	}
	Phase = PHASE_RESULTSHEET;
	UIManager = nullptr;
}

//=============================================================================
// destructor
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// Init
//=============================================================================
void CResult::Init(void)
{

	VECTOR3 PlayerRot[PLAYER_MAX] = {
		VECTOR3(0.0f,190.0f,0.0f),
		VECTOR3(0.0f,210.0f,0.0f),
		VECTOR3(0.0f,160.0f,0.0f),
		VECTOR3(110.0f,160.0f,0.0f),
		VECTOR3(100.0f,170.0f,0.0f),
		VECTOR3(125.0f,180.0f,0.0f),
	};
	VECTOR3 PlayerPos[PLAYER_MAX] = {
		VECTOR3(55,0,-370),
		VECTOR3(85,0,-340),
		VECTOR3(20,0,-330),
		VECTOR3(30,-2,60),
		VECTOR3(0,-2,10),
		VECTOR3(-50,-2,30)
	};

	VECTOR3 RockRot[kROCKMAX] = {
		VECTOR3(0.0f,190.0f,0.0f),
		VECTOR3(0.0f,210.0f,0.0f),
		VECTOR3(0.0f,160.0f,0.0f),
		VECTOR3(110.0f,160.0f,0.0f),
	};
	VECTOR3 RockPos[kROCKMAX] = {
		VECTOR3(-10,0,-370),
		VECTOR3(0,0,-340),
		VECTOR3(-30,0,-230),
		VECTOR3(-50,0,-330),
	};

	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(70.0f,70.0f),VECTOR2(0,0),5.0f);
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// ãÛ
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	// UIManager
	UIManager = CResultUIManager::Create();

	// SOUND
	CSoundAL::Play(CSoundAL::BGM_RESULT);

	VECTOR3 CameraPos = CAMERA_EYE;
	float CamPos = Ground->GetHeight(CameraPos);
	CameraPos.y += CamPos;
	// Camera
	CCamera* camera = CCamera::Camera(0);
	camera->SetEye(CameraPos);

	VECTOR3 CameraLook = CAMERA_LOOKAT;
	float CamLook = Ground->GetHeight(CameraLook);
	CameraLook.y += CamLook;
	camera->SetLookat(CameraLook);

	int* ranking = CManager::getRanking();
	//Rock
	for (int loop = 0;loop < kROCKMAX;loop++)
	{
		Rock[loop] = CModel::Create(CModel::ROCK,VECTOR3(0.0f,0.0f,0.0f));
		Rock[loop]->SetTex(CTexture::Texture(TEX_ROCK));
		Rock[loop]->SetScl(VECTOR3(0.2f,0.2f,0.2f));
		Rock[loop]->SetRot(RockRot[loop]);
		Rock[loop]->SetPos(RockPos[loop]);
		float pos = Ground->GetHeight(Rock[loop]->Pos());
		Rock[loop]->SetPosY(pos + PlayerPos[loop].y + 6.0f);
	}
	//Player
	for (int loop = 0;loop < PLAYER_MAX;loop++)
	{
		unsigned short PLAYER_TEX[PLAYER_MAX] = {
   TEX_YOUJO_RED,
   TEX_YOUJO_BLUE,
   TEX_YOUJO_WATER,
   TEX_YOUJO_ORANGE,
   TEX_YOUJO_WHITE,
   TEX_YOUJO_YELLOW,
		};
  if(ranking[loop] % 2 == 1)
  {
   Player[loop] = CModel::Create(CModel::RINCHAN,VECTOR3(0.0f,0.0f,0.0f));
  }
  else{
   Player[loop] = CModel::Create(CModel::YOUJO,VECTOR3(0.0f,0.0f,0.0f));
  }
		Player[loop]->SetTex(CTexture::Texture(PLAYER_TEX[ranking[loop]]));
		Player[loop]->SetRot(PlayerRot[loop]);
		Player[loop]->SetPos(PlayerPos[loop]);
		float pos = Ground->GetHeight(Player[loop]->Pos());
		Player[loop]->SetPosY(pos + PlayerPos[loop].y + 12.0f);
		Barrel[loop] = CModel::Create(CModel::TANK_BARREL,VECTOR3(0.0f,0.0f,0.0f));
		Barrel[loop]->SetTex(CTexture::Texture(PLAYER_TEX[ranking[loop]]));
		Barrel[loop]->SetPos(Player[loop]->Pos());
		Barrel[loop]->SetRot(Player[loop]->Rot());
	}

	CManager::SendChangeResult();
}

//=============================================================================
// Uninit
//=============================================================================
void CResult::Uninit(void)
{
	CObject::ReleaseAll();
}

//=============================================================================
// Update
//=============================================================================
void CResult::Update(void)
{
	VC* vc = VC::Instance();
	Sky->AddRotY(0.1f);
	Frame++;
	if (Frame % 2 == 0)
	{
		for (int cnt = 0;cnt < 10;cnt++)
		{
			CConfetti::Create();
		}
	}
	switch (Phase)
	{
	case PHASE_RESULTSHEET:

		if (vc->Trigger(COMMAND_OK)){
			CSoundAL::Play(CSoundAL::SE_ENTER);
			Phase = PHASE_END;
			UIManager->SetResultSheetDisable();
			UIManager->SetResultNumberDisable();
			UIManager->SetResultPlayerNameDisable();
		}
		break;

	case PHASE_END:
		if (vc->Trigger(COMMAND_OK)){
			CSoundAL::Play(CSoundAL::SE_ENTER);
			CManager::ChangeScene(SCENE_TITLE);
		}
		break;
	}
}


// end of fle