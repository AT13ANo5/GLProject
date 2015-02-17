//=============================================================================
//	ManagerGL�t�@�C�� [ ManagerGL.cpp ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	�C���N���[�h
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
//	�ÓI�ϐ���`
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

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	Render = nullptr;
	Keyboard = nullptr;
	Scene = nullptr;
	ChangeFlag = false;
	gameStartFlag = false;
	entryFlag = false;

	netData.ID = rand();

	Console::Initialize();
	Console::Print("Start Debug...");
}
//=============================================================================
//	�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
	Console::Finalize();
}

//=============================================================================
//	����������
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




	//	���肱������


	netWorkData.emptyFlag = false;




	FILE* fp;
	char add[256] = { '\0' };

	//	���[�U�[���̏�����
	initUserInfo();

	//	�T�[�o�[�̃A�h���X�擾
	fp = fopen("address.txt", "r");
	fscanf(fp, "%s", add);
	fclose(fp);

	//�l�b�g���[�N����
	//-----------------------------------------------------------------
	// ID�������_���ɐݒ�i�{���Ȃ�΃��[�UID����ݒ肷�ׂ��j
	srand(timeGetTime());
	netData.ID = rand();

	//	Winsock�̏�����
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)
	{
		printf("Winsock�̏��������s\n�G���[�R�[�h : %d\n", WSAGetLastError());
	}

	//	���M���p�ϐ��Q����
	//-------------------------------------------------
	//	�\�P�b�g�̐���
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	���M��A�h���X
	sendAddress.sin_port = htons(3000);
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_addr.S_un.S_addr = inet_addr(add);//�}���`�L���X�g�A�h���X

	int param = 1;
	int ret = setsockopt(sendSock, SOL_SOCKET, SO_BROADCAST, (char*)&param, sizeof(param));
	if (ret != 0)
		ret = WSAGetLastError();
	//-------------------------------------------------

	//	��M���p�ϐ��Q����
	//-------------------------------------------------
	//	�\�P�b�g�̐���
	recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	��M��̃A�h���X
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

	// ��M�X���b�h�J�n
	_beginthreadex(NULL, 0, &CManager::recvUpdate, NULL, NULL, NULL);
	//
	//-----------------------------------------------------------------

	//	���肱���܂�




}
//=============================================================================
//	���[�U�[��񃊃Z�b�g����
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
//	����o�C���h�֐�
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
//	�G���g���[����
//=============================================================================
void CManager::SendEntry()
{
	NET_DATA data;
	data.type = DATA_TYPE_ENTRY;
	data.servID = SERV_ID;

	sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
}
//=============================================================================
//	�Q�[���J�n�t���O���M����
//=============================================================================
void CManager::sendGameStart()
{
	//	�������ŏ�ʃN���C�A���g�ł���΁A�Q�[���J�n��������
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
//	�e���˃t���O���M����
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
//	�ʒu���M����
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
//	��]���M����
//=============================================================================
//void CManager::SendRot(float _X, float _Y, float _Z, float _rot, float _yRotation)
void CManager::SendRot(float _rotY)
{
	NET_DATA data;
	data.type = DATA_TYPE_ROT;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;

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
//	��]���M����
//=============================================================================
void CManager::SendCannonRot(VECTOR3 _rot)
{
	NET_DATA data;
	data.type = DATA_TYPE_CANNONROT;
	data.servID = SERV_ID;
	data.charNum = netData.charNum;
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
	userInfo[_id].kill = _death;

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
		for (int compareCount = PLAYER_MAX; compareCount > sortCount; compareCount--)
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
//	�X���b�h�p�̍X�V����
//=============================================================================
unsigned __stdcall CManager::recvUpdate(void *p)
{
	int rett;
	NET_DATA data;

	while (true)
	{
		// �f�[�^��M
		rett = recv(recvSock, (char*)&data, sizeof(data), 0);

		if (rett == SOCKET_ERROR)
		{
			Console::Print("network Error!!\n");
		}
		else
		{
			if (data.servID == SERV_ID)
			{
				// �f�[�^�^�C�v���
				switch (data.type)
				{
				case DATA_TYPE_POS:

					if (gameStartFlag == true)
					{
						//	�f�[�^�^�C�v�ɉ����ăv���C���[�֏����Z�b�g
						CGame::SetPlayerState(data, DATA_TYPE_POS);

						if (data.data_pos.posX > 1000.0f || data.data_pos.posX < -1000.0f ||
							data.data_pos.posZ > 1000.0f || data.data_pos.posZ < -1000.0f ||
							data.data_pos.posY > 500.0f || data.data_pos.posY < -1000.0f)
						{
							Console::Print("posError!!\n");
						}

						//	�ʒu���Z�b�g
						userInfo[data.charNum].pos.x = data.data_pos.posX;
						userInfo[data.charNum].pos.y = data.data_pos.posY;
						userInfo[data.charNum].pos.z = data.data_pos.posZ;
					}

					break;

				case DATA_TYPE_ROT:

					if (gameStartFlag == true)
					{
						//	�f�[�^�^�C�v�ɉ����ăv���C���[�֏����Z�b�g
						CGame::SetPlayerState(data, DATA_TYPE_ROT);

						//	��]���Z�b�g
						//userInfo[data.charNum].rot.y = data.data_rot.yRotation;
						userInfo[data.charNum].rot.y = data.data_rot.rotY;
					}

					break;

				case DATA_TYPE_CANNONROT:

					if (gameStartFlag == true)
					{
						//	�f�[�^�^�C�v�ɉ����ăv���C���[�֏����Z�b�g
						CGame::SetPlayerState(data, DATA_TYPE_CANNONROT);

						//	��]���Z�b�g
						userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
						userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
						userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;
					}

					break;

				case DATA_TYPE_CANNON:

					if (gameStartFlag == true)
					{
						//	�f�[�^�^�C�v�ɉ����ăv���C���[�֏����Z�b�g
						CGame::SetPlayerState(data, DATA_TYPE_CANNON);

						userInfo[data.charNum].cannon = data.data_cannon.flag;
					}

					break;

				case DATA_TYPE_PAUSE:

					if (gameStartFlag == true)
					{
						//	�|�[�Y���ɏ����擾
						userInfo[data.charNum].kill = data.data_pause.kill;
						userInfo[data.charNum].death = data.data_pause.death;
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
						if (entryFlag == false)
						{
							//	�G���g���[��������
							//	���ʔԍ����擾
							netData.charNum = data.charNum;
							entryFlag = true;
							userInfo[netData.charNum].entryFlag = true;

							//	�G���g���[���Z�b�g
							CConnection::setEntry(data.charNum);

							//	���̎Q���҂̃G���g���[�����Q�b�g����
							data.type = DATA_TYPE_GET_ENTRY;
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAddress, sizeof(sendAddress));
						}
						else
						{
							CSoundAL::Play(CSoundAL::SE_ENTRY);
							CConnection::setEntry(data.charNum);
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
						//	���������܂��Ă���
						netWorkData.emptyFlag = true;
					}

					break;

				case DATA_TYPE_CHANGE_GAME:

					//	�Q�[���֑J��
					if (data.charNum == 0)
					{
						if (netData.charNum != 0)
						{
							CSoundAL::Play(CSoundAL::SE_GAME_START);
							CManager::ChangeScene(SCENE_GAME);
						}
					}

					break;

				case DATA_TYPE_CHANGE_RESULT:

					//	�Q�[���֑J��
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
//	�I������
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
//	�X�V����
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
//	�`�揈��
//=============================================================================
void CManager::Draw(void)
{
	CCamera::Set(0);
	Render->Draw();
}
//=============================================================================
//	�V�[���ύX����
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