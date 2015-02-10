//=============================================================================
//	ManagerGLファイル [ ManagerGL.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================
#ifndef _MANAGER_GL_H_
#define _MANAGER_GL_H_

//*****************************************************************************
//	インクルード
//*****************************************************************************
#include "main.h"
#include "Renderer.h"
#include "netData.h"
class CRenderer;
class CScene;
class CCamera;
class CLight;
class CKeyboard;

//*****************************************************************************
//	構造体定義
//*****************************************************************************
struct LIGHT
{
	VECTOR4 Pos;
	COLOR Diffuse;
	COLOR Ambient;
	COLOR Specular;
};

struct NETWORK_DATA
{
	bool emptyFlag;
};

//*****************************************************************************
//	列挙体定義
//*****************************************************************************
enum
{
	SCENE_SPLASH = 0,
	SCENE_TITLE,
	SCENE_CONNECTION,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
};

//*****************************************************************************
//	列挙体定義
//*****************************************************************************
class CManager
{
public:
	CManager();
	virtual ~CManager();
	void Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(HWND hWnd);
	void Update(void);
	void Draw(void);

	static void ChangeScene(short next);
	static void SendEntry();
	static void SendPos(VECTOR3 _pos);
	static void SendRot(VECTOR3 _rot);
	static void SendCannon(bool _flag);
	static void sendGameStart();
	static NETWORK_DATA* getNetWorkData()
	{
		return &netWorkData;
	}
	static NET_DATA* getNetData()
	{
		return &netData;
	}

	CRenderer* GetRenderer(void){ return Render; }

	static NET_DATA netData;
	static USER_INFO userInfo[CHARACTER_MAX];
	static SOCKET recvSock;
	static SOCKADDR_IN recvAddress;
	static SOCKET sendSock;
	static SOCKADDR_IN sendAddress;
	static WSADATA wsaData;
	static NETWORK_DATA netWorkData;
	static bool gameStartFlag;

private:

	bool myBind(SOCKET* _socket, SOCKADDR_IN* _sockAdd);
	void initUserInfo();

	static unsigned __stdcall recvUpdate(void *p);

	static CScene* Scene;
	CRenderer* Render;
	CCamera* pCamera;
	CLight* Light;
	CKeyboard* Keyboard;

	static short NextScene;
	static bool ChangeFlag;

	
};

#endif

//	EOF