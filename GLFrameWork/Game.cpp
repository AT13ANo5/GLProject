#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "CPlayer.h"

CPlayer* g_player;
CModel* g_model;

CGame::CGame()
{

}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	
	//CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	//polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	//polygon->SetColor(GREEN(1.0f));
	CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));

	// 2014-12-01
	// 宗像：テスト
	g_player = CPlayer::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	g_player->SetTex(CTexture::Texture(TEX_MIKU));
	g_player->SetScl(VECTOR3(60.0f, 60.0f, 60.0f));
	//g_player->SetRotY(180.0f);
	
	/*
	g_model = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	//g_model->SetTex(CTexture::Texture(TEX_MIKU));
	g_model->SetScl(VECTOR3(60.0f, 60.0f, 60.0f));
	g_model->SetRotY(180.0f);
	*/
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