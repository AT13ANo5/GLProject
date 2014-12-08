#include "ManagerGL.h"
#include "Object.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Mouse.h"
#include "Keyboard.h"

#include "Scene.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Fade.h"

short	CManager::NextScene = SCENE_TITLE;
bool	CManager::ChangeFlag = false;

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

	Scene = new CTitle();
	Scene->Init();
	CFade::Set(0,5);

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
			Scene = new CGame;
			break;
		case SCENE_RESULT:
			Scene = new CResult;
			break;
		default:
			break;
		}
		if (Scene != nullptr)
		{
			CFade::Set(0,90);
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
	if (CFade::Instance().State() == CFade::FADE_NONE)
	{
		CFade::Set(1.0f,30);
		NextScene = next;
		ChangeFlag = true;
	}
}