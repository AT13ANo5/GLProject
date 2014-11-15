#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Polygon3D.h"
#include "Texture.h"

CGame::CGame()
{

}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	
	CBillboard* bill = CBillboard::Create(VECTOR3(0,0,0),VECTOR2(200.0f,200.0f),VECTOR3(0,0,0),WHITE(1.0f));
	bill->SetTex(CTexture::Texture(TEX_TEST));

	bill = CBillboard::Create(VECTOR3(0,0,200.0f),VECTOR2(200.0f,200.0f),VECTOR3(0,0,0),WHITE(0.5f));
	bill->SetTex(CTexture::Texture(TEX_TEST));

	CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	polygon->SetColor(GREEN(1.0f));

	
}

void CGame::Uninit(void)
{
	CObject::ReleaseAll();
}

void CGame::Update(void)
{
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}