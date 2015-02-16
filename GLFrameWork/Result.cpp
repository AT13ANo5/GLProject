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
#include "Keyboard.h"
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
// 静的メンバ変数
const float CResult::RADIUS_SKY = 500.0f;		// 空の半径

//=============================================================================
// macro
//=============================================================================
namespace{
	const VECTOR3 CAMERA_EYE		 = VECTOR3(50.0f, 50.0f, 200.0f);
	const VECTOR3 CAMERA_LOOKAT	 = VECTOR3(0.0f, 0.0f, 0.0f);
}


//=============================================================================
// constructor
//=============================================================================
CResult::CResult()
{
	Ground = nullptr;
	Sky = nullptr;
  Player = nullptr;
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

	// 地形
	//Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	//Ground->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_WHITE));
	Ground = CMeshGround::Create(VECTOR3(0.0f, -100.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// 空
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

  // UIManager
  UIManager = CResultUIManager::Create();

	// SOUND
	CSoundAL::Play(CSoundAL::BGM_RESULT);

	// Camera
	CCamera* camera = CCamera::Camera(0);
	camera->SetEye(CAMERA_EYE);
	camera->SetLookat(CAMERA_LOOKAT);
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
	Frame++;
	if (Frame % 2 == 0)
	{
		VECTOR3 pos = VECTOR3(0,0,400.0f);
		pos.x = rand()%101-50.0f;
		pos.y = 300.0f;
		pos.z = rand()%101-50.0f;
		for (int cnt = 0;cnt < 10;cnt++)
		{
			CConfetti::Create();
		}
	}
	switch (Phase)
	{
	case PHASE_RESULTSHEET:

    if (CKeyboard::GetTrigger(DIK_RETURN)){
		CSoundAL::Play(CSoundAL::SE_ENTER);
      Phase = PHASE_END;
      UIManager->SetResultSheetDisable();
      UIManager->SetResultNumberDisable();
      UIManager->SetResultPlayerNameDisable();
    }
		break;

	case PHASE_END:
		if (CKeyboard::GetTrigger(DIK_RETURN)){
			CSoundAL::Play(CSoundAL::SE_ENTER);
			CManager::ChangeScene(SCENE_TITLE);
		}
		break;
	}
}


// end of fle