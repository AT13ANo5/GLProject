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

// 静的メンバ変数
const float CResult::RADIUS_SKY = 500.0f;   // 空の半径
CResult::CResult()
{
	Ground = nullptr;
	Sky = nullptr;
  Player = nullptr;
  Logo = nullptr;
	Phase = PHASE_RESULTSHEET;
  UIManager = nullptr;
}

CResult::~CResult()
{

}

void CResult::Init(void)
{
  Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 300.0f, 0), VECTOR2(512.0f, 128.0f));
  Logo->SetTex(CTexture::Texture(TEX_RESULT_LOGO));

  // 地形生成
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

  // UIManager
  UIManager = CResultUIManager::Create();
  CSoundAL::Play(CSoundAL::BGM_RESULT);
}

void CResult::Uninit(void)
{
	CObject::ReleaseAll();
}

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
      Phase = PHASE_END;
      UIManager->SetResultSheetDisable();
      UIManager->SetResultNumberDisable();
      UIManager->SetResultPlayerNameDisable();
    }
		break;

	case PHASE_END:
		if (CKeyboard::GetTrigger(DIK_RETURN)){
			CManager::ChangeScene(SCENE_TITLE);
		}
		break;
	}
}