#include "Title.h"
#include "Polygon2D.h"
#include "Texture.h"
#include "Keyboard.h"
#include "ManagerGL.h"
#include "Model.h"
#include "MeshGround.h"
#include "MeshSphere.h"

// static member
const float CTitle::RADIUS_SKY = 500.0f;   // ‹ó‚Ì”¼Œa

CTitle::CTitle()
{

}

CTitle::~CTitle()
{

}

void CTitle::Init(void)
{
  // Logo
	Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/3.0f,0),VECTOR2(500.0f,250.0f));
	Logo->SetTex(CTexture::Texture(TEX_TITLELOGO));

  // Player
  Player = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
  Player->SetTex(CTexture::Texture(TEX_MIKU));
  Player->SetRot(0.0f, 180.0f, 0.0f);
  Player->SetScl(20.0f, 20.0f, 20.0f);

  // Ground
  Ground = nullptr;
  Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(50.0f, 50.0f), VECTOR2(20.0f, 20.0f));
  Ground->SetTex(CTexture::Texture(TEX_FIELD));

  // Sky
  Sky = nullptr;
  Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
  Sky->SetTex(CTexture::Texture(TEX_MIKU));

  
}

void CTitle::Uninit(void)
{
	if (Logo != nullptr)
	{
		Logo->Release();
  }
  if (Player != nullptr)
  {
    Player->Release();
  }
}

void CTitle::Update(void)
{
  if (CKeyboard::GetPress(DIK_W))
  {
    Player->AddPosZ(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_S))
  {
    Player->AddPosZ(-1.0f);
  }
  if (CKeyboard::GetPress(DIK_A))
  {
    Player->AddRotY(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_D))
  {
    Player->AddRotY(-1.0f);
  }
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

