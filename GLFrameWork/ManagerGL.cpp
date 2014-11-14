#include "ManagerGL.h"
#include "Object.h"
#include "Polygon2D.h"
#include "Polygon3D.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Effect2D.h"
#include "Model.h"
#include "MeshFiled.h"
#include "MeshCylinder.h"
#include "MeshSphere.h"
#include "Camera.h"
#include "Light.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Scene.h"
#include "Title.h"
#include "Fade.h"

CManager::CManager()
{
	Render = nullptr;
	Mouse = nullptr;
	Keyboard = nullptr;
	Scene = nullptr;
	ChangeFlag = false;

	Console::Initialize();
	Console::Print("Start Debug...");
}
CManager::~CManager()
{
	Console::Finalize();
}

void CManager::Init(HINSTANCE hInstance,HWND hWnd)
{
	Render = new CRenderer(hWnd);

	pCamera = new CCamera;
	pCamera->Init(0,30.0f,500.0f,0,0,0);
	CModel::Initialize();
	CTexture::Initialize();
	Light = new CLight;
	Light->Create(VECTOR4(1.0f,100.0f,-200.0f,0));
	Mouse = new CMouse;
	Mouse->Init(hInstance,hWnd);
	Keyboard = new CKeyboard;
	Keyboard->Init(hInstance,hWnd);

	NextScene = SCENE_TITLE;

	Scene = new CTitle();
	Scene->Init();
	CFade::Set(0,30);

}

void CManager::Uninit(HWND hWnd)
{
	if (Scene != nullptr)
	{
		Scene->Uninit();
		delete Scene;
		Scene = nullptr;
	}
	CRenderer::Uninit(hWnd);
	delete Render;
	Render=NULL;

	CModel::Finalize();
	CTexture::Finalize();

	
	if(Mouse != nullptr)
	{
		Mouse->Uninit();
		delete Mouse;
		Mouse = nullptr;
	}
	if(Keyboard != nullptr)
	{
		Keyboard->Uninit();
		delete Keyboard;
		Keyboard = nullptr;
	}

	if(pCamera!=nullptr)
	{
		delete pCamera;
		pCamera = nullptr;
	}
	if(Light != nullptr)
	{
		delete Light;
	}
	
}

void CManager::Update(void)
{
	Mouse->Update();
	Keyboard->Update();
	pCamera->Update();
	Light->Update();
	Render->Update();

	Scene->Update();

	if (CFade::Instance().State() == CFade::FADE_NONE && ChangeFlag)
	{
		Scene->Uninit();
		delete Scene;
		Scene = nullptr;

		switch (NextScene)
		{
		case SCENE_TITLE:
			Scene = new CTitle;
			break;
		case SCENE_GAME:

			break;
		case SCENE_RESULT:

			break;
		default:
			break;
		}
		if (Scene != nullptr)
		{
			CFade::Set(0,30);
			Scene->Init();
		}
		ChangeFlag = false;
	}
}

void CManager::Draw(void)
{
	pCamera->Set();
	Render->Draw();
}

void CManager::ChangeScene(short next)
{
	CFade::Set(1.0f,30);
	NextScene = next;
	ChangeFlag = true;
}