#include "main.h"
#include "Result.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Polygon2D.h"
#include "Model.h"
#include "MeshGround.h"
#include "MeshSphere.h"

// 静的メンバ変数
const float CResult::RADIUS_SKY = 500.0f;   // 空の半径


CResult::CResult()
{
  Ground = nullptr;
  Sky = nullptr;
}

CResult::~CResult()
{

}

void CResult::Init(void)
{
	CPolygon2D* polygon = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH/2.0f,SCREEN_HEIGHT/2.0f-100.0f,0),VECTOR2(512.0f,128.0f));
	polygon->SetTex(CTexture::Texture(TEX_RESULT_LOGO));

  // 地形生成
  Ground = nullptr;
  Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
  Ground->SetTex(CTexture::Texture(TEX_FIELD));

  // 空生成
  Sky = nullptr;
  Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
  Sky->SetTex(CTexture::Texture(TEX_MIKU));
}

void CResult::Uninit(void)
{
	CObject::ReleaseAll();
}

void CResult::Update(void)
{

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_TITLE);
	}
}