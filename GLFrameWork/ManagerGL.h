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
class VC;

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
	static void SendPos(VECTOR3 _pos, int _id);
	//static void SendRot(float _X, float _Y, float _Z, float _rot, float _yRotation);
	static void SendRot(float _rotY, int _id);
	static void SendCannonRot(VECTOR3 _rot, int _id);
	static void SendCannon(bool _flag, int _id);
	static void sendGameStart();
	static void SendChangeGame();
	static void SendChangeResult();
	static void SendReborn(int _id);
	static void SendKillDeath(int _kill, int _death);


	static void SendKill(int _kill, int _id);
	static void SendDeath(int _death, int _id);
	static void SendDeathFlag(int _id);


	static int* getRanking();
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
	static bool entryFlag;

	static short CurrentScene;

	static bool sendEntryFlag;

private:

	bool myBind(SOCKET* _socket, SOCKADDR_IN* _sockAdd);
	void initUserInfo();

	static unsigned __stdcall recvUpdate(void *p);

	static CScene* Scene;
	CRenderer* Render;
	CCamera* pCamera;
	CLight* Light;
	VC* vc;

	static short NextScene;
	static bool ChangeFlag;

	static int ranking[PLAYER_MAX];
};

#endif

//	EOF