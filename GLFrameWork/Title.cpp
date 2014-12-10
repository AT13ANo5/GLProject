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
 Player = CModel::Create(CModel::TANK_YOUJO_RINCHAN,VECTOR3(0.0f,0.0f,0.0f));
 Player->SetTex(CTexture::Texture(TEX_YOUJO_YELLOW));
 Player->SetScl(1,1,1);
 Balel = CModel::Create(CModel::TANK_BALEL,VECTOR3(0.0f,0.0f,0.0f));
 Balel->SetTex(CTexture::Texture(TEX_YOUJO_YELLOW));
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
  if(Balel != nullptr)
  {
   Balel->Release();
  }

}

void CTitle::Update(void)
{
  if (CKeyboard::GetPress(DIK_W))
  {
   Player->AddPosZ(1.0f);
   Balel->AddPosZ(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_S))
  {
   Player->AddPosZ(-1.0f);
   Balel->AddPosZ(-1.0f);
  }
  if (CKeyboard::GetPress(DIK_A))
  {
   Player->AddRotY(1.0f);
   Balel->AddRotY(1.0f);
  }
  else if (CKeyboard::GetPress(DIK_D))
  {
   Player->AddRotY(-1.0f);
   Balel->AddRotY(-1.0f);
  }
  if(CKeyboard::GetPress(DIK_Q))
  {
   Balel->AddRotZ(-1.0f);

  }
  if(CKeyboard::GetPress(DIK_E))
  {
   Balel->AddRotZ(1.0f);

  }
   if(CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

