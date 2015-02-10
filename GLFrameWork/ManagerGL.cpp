//=============================================================================
//	ManagerGLファイル [ ManagerGL.cpp ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	インクルード
//*****************************************************************************
#include "ManagerGL.h"
#include "Object.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Keyboard.h"
#include "SoundAL.h"
#include "Scene.h"
#include "Splash.h"
#include "Title.h"
#include "Connection.h"
#include "Game.h"
#include "Result.h"
#include "Fade.h"

//*****************************************************************************
//	静的変数定義
//*****************************************************************************
short CManager::NextScene = SCENE_SPLASH;
bool CManager::ChangeFlag = false;
CScene* CManager::Scene;
NET_DATA CManager::netData;
USER_INFO CManager::userInfo[CHARACTER_MAX];
SOCKET CManager::recvSock;
SOCKADDR_IN CManager::recvAddress;
SOCKET CManager::sendSock;
SOCKADDR_IN CManager::sendAddress;
WSADATA CManager::wsaData;
NETWORK_DATA CManager::netWorkData;
bool CManager::gameStartFlag;

//=============================================================================
//	コンストラクタ
//=============================================================================
CManager::CManager()
{
	Render = nullptr;
	Keyboard = nullptr;
	Scene = nullptr;
	ChangeFlag = false;
	gameStartFlag = false;

	netData.ID = rand();

	Console::Initialize();
	Console::Print("Start Debug...");
}
//=============================================================================
//	デストラクタ
//=============================================================================
CManager::~CManager()
{
	Console::Finalize();
}

//=============================================================================
//	初期化処理
//=============================================================================
void CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	Render = new CRenderer(hWnd);

	pCamera = new CCamera;
	pCamera->Init(0, 30.0f, 1000.0f, 0, 0, 0);
	CSoundAL::Initialize();
	CModel::Initialize();
	CTexture::Initialize();
	Light = new CLight;
	Light->Create(VECTOR4(1.0f, 100.0f, -200.0f, 0));
 	Light->SetAmbient(COLOR(1.0f,1.0f,1.0f,1.0f));
	Keyboard = new CKeyboard;
	Keyboard->Init(hInstance, hWnd);

	Scene = new CSplash();
	Scene->Init();
	CFade::Set(0, 60);




	//	長崎ここから


	netWorkData.emptyFlag = false;




	FILE* fp;
	char add[256] = { '\0' };

	//	ユーザー情報の初期化
	initUserInfo();

	//	サーバーのアドレス取得
	fp = fopen("address.txt", "r");
	fscanf(fp, "%s", add);

	//ネットワーク処理
	//-----------------------------------------------------------------
	// IDをランダムに設定（本来ならばユーザID等を設定すべき）
	srand(timeGetTime());
	netData.ID = rand();

	//	Winsockの初期化
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)
	{
		printf("Winsockの初期化失敗\nエラーコード : %d\n", WSAGetLastError());
	}

	//	送信時用変数群生成
	//-------------------------------------------------
	//	ソケットの生成
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	送信先アドレス
	sendAddress.sin_port = htons(3000);
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_addr.S_un.S_addr = inet_addr(add);//マルチキャストアドレス

	int param = 1;
	int ret = setsockopt(sendSock, SOL_SOCKET, SO_BROADCAST, (char*)&param, sizeof(param));
	if (ret != 0)
		ret = WSAGetLastError();
	//-------------------------------------------------

	//	受信時用変数群生成
	//-------------------------------------------------
	//	ソケットの生成
	recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	受信先のアドレス
	recvAddress.sin_port = htons(2000);
	recvAddress.sin_family = AF_INET;
	recvAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int recvAddLength = sizeof(recvAddress);

	myBind(&recvSock, &recvAddress);

	ip_mreq mreq;
	mreq.imr_multiaddr.S_un.S_addr = inet_addr("239.0.0.23");
	mreq.imr_interface.S_un.S_addr = INADDR_ANY;
	ret = setsockopt(recvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&mreq, sizeof(mreq));
	if (ret != 0)
		ret = WSAGetLastError();
	//-------------------------------------------------

	// 受信スレッド開始
	_beginthreadex(NULL, 0, &CManager::recvUpdate, NULL, NULL, NULL);
	//
	//-----------------------------------------------------------------








	//	長崎ここまで




}
//=============================================================================
//	ユーザー情報リセット処理
//=============================================================================
void CManager::initUserInfo()
{
	for (int count = 0; count < CHARACTER_MAX; count++)
	{
		memset(&userInfo[count].fromaddr, 0, sizeof(userInfo[count].fromaddr));
		userInfo[count].entryFlag = false;
		userInfo[count].death = 0;
		userInfo[count].kill = 0;
		userInfo[count].pos = VECTOR3(0.0f, 0.0f, 0.0f);
		userInfo[count].rot = VECTOR3(0.0f, 0.0f, 0.0f);
		userInfo[count].cannon = false;
	}
}
//=============================================================================
//	自作バインド関数
//=============================================================================
bool CManager::myBind(SOCKET* _socket, SOCKADDR_IN* _sockAdd)
{
	int ret;

	for (;;)
	{
		int portAdd = 1;

		ret = bind(*_socket, (sockaddr*)_sockAdd, sizeof(*_sockAdd));
		ret = WSAGetLastError();

		if (ret == WSAEADDRINUSE)
		{
			_sockAdd->sin_port = htons(20000 + portAdd);
			portAdd++;
			continue;
		}
		else
			break;
	}
	return true;
}
//=============================================================================
//	エントリー処理
//=============================================================================
void CManager::SendEntry()
{
	NET_DATA data;
	data.type = DATA_TYPE_ENTRY;
	data.servID = SERV_ID;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	ゲーム開始フラグ送信処理
//=============================================================================
void CManager::sendGameStart()
{
	//	自分が最上位クライアントであれば、ゲーム開始を告げる
	if (netData.charNum == 0)
	{
		NET_DATA data;
		data.type = DATA_TYPE_GAME_START;
		data.servID = SERV_ID;

		sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
	}
}
//=============================================================================
//	弾発射フラグ送信処理
//=============================================================================
void CManager::SendCannon(bool _flag)
{
	NET_DATA data;
	data.type = DATA_TYPE_CANNON;
	data.charNum = netData.charNum;
	data.servID = SERV_ID;
	data.data_cannon.flag = _flag;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	位置送信処理
//=============================================================================
void CManager::SendPos(VECTOR3 _pos)
{
	NET_DATA data;
	data.type = DATA_TYPE_POS;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;
	data.data_pos.posX = _pos.x;
	data.data_pos.posY = _pos.y;
	data.data_pos.posZ = _pos.z;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	回転送信処理
//=============================================================================
void CManager::SendRot(VECTOR3 _rot)
{
	NET_DATA data;
	data.type = DATA_TYPE_ROT;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;
	data.data_rot.rotX = _rot.x;
	data.data_rot.rotY = _rot.y;
	data.data_rot.rotZ = _rot.z;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	スレッド用の更新処理
//=============================================================================
unsigned __stdcall CManager::recvUpdate(void *p)
{
	int rett;
	NET_DATA data;

	while (true)
	{
		// データ受信
		rett = recv(recvSock, (char*)&data, sizeof(data), 0);

		if (rett == SOCKET_ERROR)
		{
			Console::Print("network Error!!\n");
		}
		else
		{
			if (data.servID == SERV_ID)
			{
				// データタイプ解析
				switch (data.type)
				{
				case DATA_TYPE_POS:

					if (gameStartFlag == true)
					{
						//	データタイプに応じてプレイヤーへ情報をセット
						CGame::SetPlayerState(data, DATA_TYPE_POS);

						//	位置情報セット
						userInfo[data.charNum].pos.x = data.data_pos.posX;
						userInfo[data.charNum].pos.y = data.data_pos.posY;
						userInfo[data.charNum].pos.z = data.data_pos.posZ;
					}

					break;

				case DATA_TYPE_ROT:

					if (gameStartFlag == true)
					{
						//	データタイプに応じてプレイヤーへ情報をセット
						CGame::SetPlayerState(data, DATA_TYPE_ROT);

						//	回転情報セット
						userInfo[data.charNum].rot.x = data.data_rot.rotX;
						userInfo[data.charNum].rot.y = data.data_rot.rotY;
						userInfo[data.charNum].rot.z = data.data_rot.rotZ;
					}

					break;

				case DATA_TYPE_CANNON:

					if (gameStartFlag == true)
					{
						//	データタイプに応じてプレイヤーへ情報をセット
						CGame::SetPlayerState(data, DATA_TYPE_CANNON);
					}

					break;

				case DATA_TYPE_PAUSE:

					if (gameStartFlag == true)
					{
						//	ポーズ時に情報を取得
						userInfo[data.charNum].kill = data.data_pause.kill;
						userInfo[data.charNum].death = data.data_pause.death;
					}

					break;

				case DATA_TYPE_ENTRY:

					if (gameStartFlag == false)
					{
						//	エントリー処理完了
						//	識別番号を取得
						netData.charNum = data.charNum;
					}

					break;

				case DATA_TYPE_EMPTY:

					if (gameStartFlag == false)
					{
						//	部屋が埋まっている
						netWorkData.emptyFlag = true;
					}

					break;
				}
			}
			else
			{
				Console::Print("networkID Not Equal!!\n");
			}
		}
	}

	return NULL;
}
//=============================================================================
//	終了処理
//=============================================================================
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
	Render = NULL;

	CSoundAL::Finalize();
	CModel::Finalize();
	CTexture::Finalize();


	if (Keyboard != nullptr)
	{
		Keyboard->Uninit();
		delete Keyboard;
		Keyboard = nullptr;
	}

	CCamera::ReleaseAll();

	if (Light != nullptr)
	{
		delete Light;
	}

}
//=============================================================================
//	更新処理
//=============================================================================
void CManager::Update(void)
{
#ifdef _DEBUG
	if(CKeyboard::GetTrigger(DIK_0))
	{
		(Console::Active()) ? Console::SetActive(false) : Console::SetActive(true);
	}

#endif
	Keyboard->Update();
	CSoundAL::UpdateAll();
	CCamera::UpdateAll();
	Light->Update();
	Render->Update();

	Scene->Update();

	if (CFade::Instance().State() == CFade::FADE_NONE && ChangeFlag)
	{
		Scene->Uninit();
		delete Scene;
		Scene = nullptr;

		if (NextScene != SCENE_GAME&&pCamera == nullptr)
		{
			pCamera = new CCamera;
			pCamera->Init(0, 30.0f, 500.0f, 0, 0, 0);
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
			CCamera::ReleaseAll();
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
			CFade::Set(0, 30);
			Scene->Init();
		}
		ChangeFlag = false;
	}
}

//=============================================================================
//	描画処理
//=============================================================================
void CManager::Draw(void)
{
	CCamera::Set(0);
	Render->Draw();
}
//=============================================================================
//	シーン変更処理
//=============================================================================
void CManager::ChangeScene(short next)
{
	if (CFade::Instance().State() == CFade::FADE_NONE&&ChangeFlag == false)
	{
		CFade::Set(1.0f, 30);
		CSoundAL::FadeAll(20);
		NextScene = next;
		ChangeFlag = true;
	}
}