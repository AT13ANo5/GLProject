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
#include "Input/VC.h"
#include "Input/Keyboard.h"
#include "SoundAL.h"
#include "Scene.h"
#include "Splash.h"
#include "Title.h"
#include "Connection.h"
#include "Game.h"
#include "Result.h"
#include "Fade.h"
#include "Random.h"

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
bool CManager::entryFlag;
int CManager::ranking[PLAYER_MAX];
short CManager::CurrentScene = SCENE_SPLASH;
bool CManager::sendEntryFlag;

//=============================================================================
//	コンストラクタ
//=============================================================================
CManager::CManager()
{
	Render = nullptr;
	vc = nullptr;
	Scene = nullptr;
	ChangeFlag = false;
	gameStartFlag = false;
	entryFlag = false;
	sendEntryFlag = false;

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
	RandomMT::initRand();

	Light = new CLight;
	Light->Create(VECTOR4(100.0f,120.0f,-200.0f,0));
	Light->SetAmbient(COLOR(0.85f,0.9f,1.0f,1.0f));
	Light->SetDiffuse(COLOR(1.0f,0.95f,0.85f,1.0f));
	vc = VC::Instance();
	vc->Init(hWnd);

	Scene = new CSplash();
	Scene->Init();
	CFade::Set(0, 60);

	CurrentScene = SCENE_SPLASH;




	//	長崎ここから
	netWorkData.emptyFlag = false;

	FILE* fp;
	char recvAdd[256] = { '\0' };
	char sendAdd[256] = { '\0' };

	//	ユーザー情報の初期化
	initUserInfo();

	//	受信アドレス取得
	fp = fopen("recvAddress.txt", "r");
	fscanf(fp, "%s", recvAdd);
	fclose(fp);

	//	送信アドレス取得
	fp = fopen("sendAddress.txt", "r");
	fscanf(fp, "%s", sendAdd);
	fclose(fp);

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

	/*//	送信時用変数群生成
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
	//-------------------------------------------------*/

	//	送信時用変数群生成
	//-------------------------------------------------
	//	ソケットの生成
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	送信先アドレス
	sendAddress.sin_port = htons(3000);
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_addr.S_un.S_addr = inet_addr(sendAdd);//マルチキャストアドレス

	int param = 1;
	int ret = setsockopt(sendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&param, sizeof(param));
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
	mreq.imr_multiaddr.S_un.S_addr = inet_addr(recvAdd);
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

	sendEntryFlag = false;
	ChangeFlag = false;
	gameStartFlag = false;
	entryFlag = false;
	sendEntryFlag = false;

	netData.charNum = 0;
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

	sendEntryFlag = true;

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
		data.charNum = 0;

		sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
	}
}
//=============================================================================
//	弾発射フラグ送信処理
//=============================================================================
void CManager::SendCannon(bool _flag, int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_CANNON;
	data.charNum = _id;
	data.servID = SERV_ID;
	data.data_cannon.flag = _flag;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	位置送信処理
//=============================================================================
void CManager::SendPos(VECTOR3 _pos, int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_POS;
	data.servID = SERV_ID;
	data.charNum = _id;
	data.data_pos.posX = _pos.x;
	data.data_pos.posY = _pos.y;
	data.data_pos.posZ = _pos.z;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	回転送信処理
//=============================================================================
//void CManager::SendRot(float _X, float _Y, float _Z, float _rot, float _yRotation)
void CManager::SendRot(float _rotY, int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_ROT;
	data.servID = SERV_ID;
	data.charNum = _id;

	data.data_rot.rotY = _rotY;

/*#ifdef ROT_QUART
	data.data_rot.X = _X;
	data.data_rot.Y = _Y;
	data.data_rot.Z = _Z;
	data.data_rot.rot = _rot;
	data.data_rot.rot = _yRotation;
#endif

#ifdef ROT_NORMAL
	data.data_rot.rotX = _X;
	data.data_rot.rotY = _Y;
	data.data_rot.rotZ = _Z;
#endif*/

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	回転送信処理
//=============================================================================
void CManager::SendCannonRot(VECTOR3 _rot, int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_CANNONROT;
	data.servID = SERV_ID;
	data.charNum = _id;
	data.data_cannonRot.rotX = _rot.x;
	data.data_cannonRot.rotY = _rot.y;
	data.data_cannonRot.rotZ = _rot.z;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}

void CManager::SendChangeGame()
{
	NET_DATA data;
	data.type = DATA_TYPE_CHANGE_GAME;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
void CManager::SendChangeResult()
{
	NET_DATA data;
	data.type = DATA_TYPE_CHANGE_RESULT;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
void CManager::SendReborn(int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_SEND_REBORN;
	data.servID = SERV_ID;
	data.charNum = _id;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
void CManager::SendKillDeath(int _kill, int _death)
{
	NET_DATA data;
	data.type = DATA_TYPE_KILL;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;
	data.data_killDeath.value = _kill;
	userInfo[netData.charNum].kill = _kill;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));


	data.type = DATA_TYPE_DEATH;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;
	data.data_killDeath.value = _death;
	userInfo[netData.charNum].death = _death;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
void CManager::SendDeathFlag(int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_SEND_DEATH;
	data.servID = SERV_ID;
	data.charNum = _id;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
void CManager::SendKill(int _kill, int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_KILL;
	data.servID = SERV_ID;
	data.charNum = _id;
	data.data_killDeath.value = _kill;
	userInfo[_id].kill = _kill;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
void CManager::SendDeath(int _death, int _id)
{
	NET_DATA data;
	data.type = DATA_TYPE_DEATH;
	data.servID = SERV_ID;
	data.charNum = _id;
	data.data_killDeath.value = _death;
	userInfo[_id].death = _death;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
int* CManager::getRanking()
{
	for (int count = 0; count < PLAYER_MAX; count++)
	{
		ranking[count] = 0;
	}

	int score[PLAYER_MAX];
	int scoreID[PLAYER_MAX];

	for (int count = 0; count < PLAYER_MAX; count++)
	{
		score[count] = userInfo[count].kill - userInfo[count].death;
		scoreID[count] = count;
	}



	for (int sortCount = 0; sortCount < PLAYER_MAX; sortCount++)
	{
		for (int compareCount = PLAYER_MAX - 1; compareCount > sortCount; compareCount--)
		{
			if (score[compareCount] > score[compareCount - 1])
			{
				int work = score[compareCount];
				score[compareCount] = score[compareCount - 1];
				score[compareCount - 1] = work;


				work = scoreID[compareCount];


				scoreID[compareCount] = scoreID[compareCount - 1];
				scoreID[compareCount - 1] = work;
			}
		}
	}

	for (int count = 0; count < PLAYER_MAX; count++)
	{
		ranking[count] = scoreID[count];
	}

	return ranking;
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
						//userInfo[data.charNum].rot.y = data.data_rot.yRotation;
						userInfo[data.charNum].rot.y = data.data_rot.rotY;
					}

					break;

				case DATA_TYPE_CANNONROT:

					if (gameStartFlag == true)
					{
						//	データタイプに応じてプレイヤーへ情報をセット
						CGame::SetPlayerState(data, DATA_TYPE_CANNONROT);

						//	回転情報セット
						userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
						userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
						userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;
					}

					break;

				case DATA_TYPE_CANNON:

					if (gameStartFlag == true)
					{
						//	データタイプに応じてプレイヤーへ情報をセット
						CGame::SetPlayerState(data, DATA_TYPE_CANNON);

						userInfo[data.charNum].cannon = data.data_cannon.flag;
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

				case DATA_TYPE_TIMER:

					if (CManager::CurrentScene == SCENE_GAME)
					{
						if (CGame::gamePhase == CGame::PHASE_3)
						{
							CGame::gamePhaseCnt = 60;
						}
						else if (CGame::gamePhase == CGame::PHASE_2)
						{
							CGame::gamePhaseCnt = 120;
						}
						else if (CGame::gamePhase == CGame::PHASE_1)
						{
							CGame::gamePhaseCnt = 180;
						}
						else if (CGame::gamePhase == CGame::PHASE_START)
						{
							CGame::gamePhaseCnt = 240;
						}
						else if (CGame::gamePhase == CGame::PHASE_START_FIN)
						{
							CGame::gamePhaseCnt = 300;
						}
						else
							CGame::subTimer();
					}

					break;

				case DATA_TYPE_KILL:

					if (gameStartFlag == true)
					{
						userInfo[data.charNum].kill = data.data_killDeath.value;

						CGame::SetPlayerState(data, DATA_TYPE_KILL);
					}

					break;

				case DATA_TYPE_DEATH:

					if (gameStartFlag == true)
					{
						userInfo[data.charNum].death = data.data_killDeath.value;

						CGame::SetPlayerState(data, DATA_TYPE_DEATH);
					}

					break;

				case DATA_TYPE_ENTRY:

					if (gameStartFlag == false)
					{
						if (sendEntryFlag == true)
						{
							if (entryFlag == false)
							{
								//	エントリー処理完了
								//	識別番号を取得
								netData.charNum = data.charNum;
								entryFlag = true;
								userInfo[netData.charNum].entryFlag = true;

								//	エントリーをセット
								CConnection::setEntry(data.charNum);
								CConnection::setTexHostPos(data.charNum);

								//	他の参加者のエントリー情報をゲットする
								data.type = DATA_TYPE_GET_ENTRY;
								sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
							}
							else
							{
								CSoundAL::Play(CSoundAL::SE_ENTRY);
								CConnection::setEntry(data.charNum);
							}
						}
					}

					break;

				case DATA_TYPE_GET_ENTRY:

					for (int count = 0; count < PLAYER_MAX; count++)
					{
						if (data.data_connection.entryFlag[count] == true)
						{
							CConnection::setEntry(count);
							userInfo[count].entryFlag = true;
						}
						else
							break;
					}

					break;

				case DATA_TYPE_EMPTY:

					if (gameStartFlag == false)
					{
						//	部屋が埋まっている
						netWorkData.emptyFlag = true;
					}

					break;

				case DATA_TYPE_CHANGE_GAME:

					//	ゲームへ遷移
					if (data.charNum == 0)
					{
						if (netData.charNum != 0)
						{
							CSoundAL::Play(CSoundAL::SE_ENTER);
							CManager::ChangeScene(SCENE_GAME);
						}
					}

					break;

				case DATA_TYPE_CHANGE_RESULT:

					//	ゲームへ遷移
					if (data.charNum == 0)
					{
						if (netData.charNum != 0)
						{
							CManager::ChangeScene(SCENE_RESULT);
						}
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


	if (vc != nullptr)
	{
		vc->Uninit();
		vc = nullptr;
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
	if(CKeyboard::Instance()->GetTrigger(DIK_0))
	{
		(Console::Active()) ? Console::SetActive(false) : Console::SetActive(true);
	}

#endif
	vc->Update();
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
			CurrentScene = SCENE_SPLASH;
			break;
		case SCENE_TITLE:
			Scene = new CTitle;
			CurrentScene = SCENE_TITLE;
			break;
		case SCENE_CONNECTION:
			Scene = new CConnection;
			CurrentScene = SCENE_CONNECTION;
			break;
		case SCENE_GAME:
			CCamera::ReleaseAll();
			pCamera = nullptr;
			Scene = new CGame;
			CurrentScene = SCENE_GAME;
			break;
		case SCENE_RESULT:
			Scene = new CResult;
			CurrentScene = SCENE_RESULT;
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
		CFade::Set(1.0f, 60);
		CSoundAL::FadeBGM(50);
		NextScene = next;
		ChangeFlag = true;
	}
}