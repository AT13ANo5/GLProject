#include "ManagerGL.h"
#include "Object.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SoundAL.h"

#include "Scene.h"
#include "Splash.h"
#include "Title.h"
#include "Connection.h"
#include "Game.h"
#include "Result.h"
#include "Fade.h"

short	CManager::NextScene = SCENE_SPLASH;
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
	CSoundAL::Initialize();
	CModel::Initialize();
	CTexture::Initialize();
	Light = new CLight;
	Light->Create(VECTOR4(1.0f,100.0f,-200.0f,0));
	Mouse = new CMouse;
	Mouse->Init(hInstance,hWnd);
	Keyboard = new CKeyboard;
	Keyboard->Init(hInstance,hWnd);

	Scene = new CSplash();
	Scene->Init();
	CFade::Set(0,60);

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

	CSoundAL::Finalize();
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

	CCamera::Release();

	if(Light != nullptr)
	{
		delete Light;
	}
	
}

void CManager::Update(void)
{
	Mouse->Update();
	Keyboard->Update();
	CSoundAL::UpdateAll();
	CCamera::UpdateCur();
	Light->Update();
	Render->Update();

	Scene->Update();

	if (CFade::Instance().State() == CFade::FADE_NONE && ChangeFlag)
	{
		Scene->Uninit();
		delete Scene;
		Scene = nullptr;

		if (NextScene != SCENE_GAME&&pCamera==nullptr)
		{
			pCamera = new CCamera;
			pCamera->Init(0,30.0f,500.0f,0,0,0);
		}

		switch (NextScene)
		{
		case SCENE_SPLASH:
			Scene = new CSplash;
			break;
		case SCENE_TITLE:
			Scene = new CTitle;
			break;
		case SCENE_CONNECTION:
			Scene = new CConnection;
			break;
		case SCENE_GAME:
			CCamera::Release();
			pCamera = nullptr;
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
			CFade::Set(0,30);
			Scene->Init();
		}
		ChangeFlag = false;
	}
}

void CManager::Draw(void)
{
	CCamera::SetCur();
	Render->Draw();
}

void CManager::ChangeScene(short next)
{
	if (CFade::Instance().State() == CFade::FADE_NONE&&ChangeFlag==false)
	{
		CFade::Set(1.0f,30);
		NextScene = next;
		ChangeFlag = true;
	}
}