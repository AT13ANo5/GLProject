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
}

void CTitle::Uninit(void)
{
	if (Logo != nullptr)
	{
		Logo->Release();
	}
}

void CTitle::Update(void)
{
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_CONNECTION);
	}
}

