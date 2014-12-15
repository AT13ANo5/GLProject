#include "main.h"
#include "Result.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Polygon2D.h"
#include "Model.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "ResultSheet.h"
#include "Fade.h"

// 静的メンバ変数
const float CResult::RADIUS_SKY = 500.0f;   // 空の半径
CResult::CResult()
{
	Ground = nullptr;
	Sky = nullptr;
	ResultSheet = nullptr;
	Phase = PHASE_RESULTSHEET;
}

CResult::~CResult()
{

}

void CResult::Init(void)
{
	CPolygon2D* polygon = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f - 300.0f,0),VECTOR2(512.0f,128.0f));
	polygon->SetTex(CTexture::Texture(TEX_RESULT_LOGO));
	// 地形生成
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(50.0f,50.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	ResultSheet = nullptr;
	ResultSheet = CResultSheet::Create(VECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f + 200.0f,0),VECTOR2(SCREEN_WIDTH*0.6f,256.0f));
	ResultSheet->SetTex(CTexture::Texture(TEX_RESULT_TEXT));
	ResultSheet->SetAlpha(0.0f);
}

void CResult::Uninit(void)
{
	CObject::ReleaseAll();
}

void CResult::Update(void)
{
	switch (Phase)
	{
	case PHASE_RESULTSHEET:
		ResultSheet->DrawEnable();
		if (CKeyboard::GetTrigger(DIK_RETURN)){
			Phase = PHASE_END;
			ResultSheet->DrawDisable();
		}
		break;

	case PHASE_END:
		if (CKeyboard::GetTrigger(DIK_RETURN)){
			CManager::ChangeScene(SCENE_TITLE);
		}
		break;
	}
}