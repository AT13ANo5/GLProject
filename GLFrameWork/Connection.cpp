#include "Connection.h"
#include "Polygon2D.h"
#include "ManagerGL.h"
#include "Keyboard.h"

CConnection::CConnection()
{
	Logo = nullptr;
}

CConnection::~CConnection()
{

}

void CConnection::Init(void)
{
	Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH/2.0f,SCREEN_HEIGHT/2.0f,0),VECTOR2(512.0f,256.0f));
	Logo->SetTex(CTexture::Texture(TEX_CONNECTION));
}

void CConnection::Uninit(void)
{
	if (Logo != nullptr)
	{
		Logo->Release();
		Logo = nullptr;
	}

}

void CConnection::Update(void)
{
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_GAME);
	}
}