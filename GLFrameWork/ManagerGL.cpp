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
CManager::CManager()
{
	Render = nullptr;
	Mouse = nullptr;
	Keyboard = nullptr;

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

	CBillboard* bill = CBillboard::Create(VECTOR3(0,0,0),VECTOR2(100.0f,100.0f),VECTOR3(0,0,0),RED(0.5f));
	bill->SetTex(CTexture::Texture(CTexture::TEX_LIGHT));
	CEffect3D::Create(VECTOR3(0,0,100.0f),VECTOR2(100.0f,100.0f),VECTOR3(0,0,0),BLUE(1.0f));
	CPolygon3D::Create(VECTOR3(0,0,-100.0f),VECTOR2(100.0f,100.0f),VECTOR3(-90.0f,0,0),GREEN(0.8f));

}

void CManager::Uninit(HWND hWnd)
{

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
}

void CManager::Draw(void)
{
	pCamera->Set();
	Render->Draw();
}