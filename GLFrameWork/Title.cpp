#include "Title.h"
#include "Polygon2D.h"
#include "Texture.h"
#include "Keyboard.h"
#include "ManagerGL.h"
#include "Model.h"

CTitle::CTitle()
{

}

CTitle::~CTitle()
{

}

void CTitle::Init(void)
{
	Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/3.0f,0),VECTOR2(500.0f,250.0f));
	Logo->SetTex(CTexture::Texture(TEX_TITLELOGO));
  Player = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
  Player->SetTex(CTexture::Texture(TEX_MIKU));
  Player->SetRot(0.0f, 180.0f, 0.0f);
  Player->SetScl(20.0f, 20.0f, 20.0f);
}

void CTitle::Uninit(void)
{
	if (Logo != NULL)
	{
		Logo->Release();
  }
  if (Player != NULL)
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
    Player->AddPosX(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_D))
  {
    Player->AddPosX(-1.0f);
  }
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

