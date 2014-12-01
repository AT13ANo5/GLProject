#include "Splash.h"
#include "ManagerGL.h"
#include "Polygon2D.h"
#include "CommonGL.h"
#include "Keyboard.h"

CSplash::CSplash()
{
	Logo = nullptr;

}

CSplash::~CSplash()
{

}

void CSplash::Init(void)
{
	Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH/2.0f,SCREEN_HEIGHT/4.0f,0),VECTOR2(512.0f,256.0f));
	Logo->SetTex(CTexture::Texture(TEX_TEAM_LOGO));
}

void CSplash::Uninit(void)
{
	Logo->Release();
	Logo = nullptr;
}

void CSplash::Update(void)
{
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_TITLE);
	}

}