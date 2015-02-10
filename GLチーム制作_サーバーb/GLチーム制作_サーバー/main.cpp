//=============================================================================
//	main�t�@�C�� [ main.cpp ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	�萔��`
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//*****************************************************************************
//	�C���N���[�h
//*****************************************************************************
#include <ws2tcpip.h>
#include <time.h>
#include<process.h>
#include "main.h"
#include "AI.h"

//*****************************************************************************
//	���C�u�����̃����N
//*****************************************************************************
#pragma comment ( lib , "ws2_32.lib" )
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
//	�v���g�^�C�v�錾
//*****************************************************************************
unsigned __stdcall aiUpdate(void *p);

//*****************************************************************************
//	�萔��`
//*****************************************************************************
const int charcterMax = 6;
const int hostNameMax = 256;
const int sendPort = 20000;
USER_INFO userInfo[charcterMax];

//=============================================================================
//	����o�C���h�֐�
//=============================================================================
bool myBind(SOCKET* _socket, SOCKADDR_IN* _sockAdd)
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
//	���[�U�[��񃊃Z�b�g����
//=============================================================================
void initUserInfo()
{
	for (int count = 0; count < charcterMax; count++)
	{
		memset(&userInfo[count].fromaddr, 0, sizeof(userInfo[count].fromaddr));
		userInfo[count].entryFlag = false;
		userInfo[count].death = 0;
		userInfo[count].kill = 0;
		userInfo[count].pos = VECTOR3(0.0f, 0.0f, 0.0f);
		userInfo[count].rot = VECTOR3(0.0f, 0.0f, 0.0f);
		userInfo[count].cannonRot = VECTOR3(0.0f, 0.0f, 0.0f);
		userInfo[count].cannon = false;
	}
}

//=============================================================================
//	���C�������֐�
//=============================================================================
int main(void)
{
	WSADATA wsaData;

	// �z�X�g���EIP�A�h���X
	char hostName[hostNameMax];
	char ip[16];
	PHOSTENT phostent;
	IN_ADDR in;

	//	���݂̃L�����N�^�[��
	int charNum = -1;

	//	Winsock�̏�����
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NULL)
	{
		printf("Winsock�̏��������s\n�G���[�R�[�h : %d\n", WSAGetLastError());

		return false;
	}

	//	�����̃z�X�g���擾
	gethostname(hostName, sizeof(hostName));
	phostent = gethostbyname(hostName);

	//	������IP�A�h���X��\���i���[�U�[�͕\������Ă���IP�A�h���X��"address.txt"�ɃZ�b�g����j
	memcpy(&in, phostent->h_addr, 4);
	sprintf_s(ip, inet_ntoa(in));
	printf("%s:%s\n", hostName, ip);

	//	���M���p�ϐ��Q����
	//-------------------------------------------------
	//	�\�P�b�g�̐���
	SOCKET sendSock;
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	���M��A�h���X
	sockaddr_in sendAdd;
	sendAdd.sin_port = htons(2000);
	sendAdd.sin_family = AF_INET;
	sendAdd.sin_addr.s_addr = inet_addr("239.0.0.23");//�}���`�L���X�g�A�h���X

	int param = 1;
	int ret = setsockopt(sendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&param, sizeof(param));
	ret = WSAGetLastError();
	//-------------------------------------------------

	//	��M���p�ϐ��Q����
	//-------------------------------------------------
	//	�\�P�b�g�̐���
	SOCKET recvSock;
	recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//	��M��̃A�h���X
	sockaddr_in recvAdd;
	recvAdd.sin_port = htons(3000);
	recvAdd.sin_family = AF_INET;
	recvAdd.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	int recvAddLength = sizeof(recvAdd);
	
	setsockopt(recvSock, SOL_SOCKET, SO_BROADCAST, (char *)&param, sizeof(param));

	myBind(&recvSock, &recvAdd);
	//-------------------------------------------------

	AI::Initialize();
	
	UINT threadID = 0;

	HANDLE ai = 0;
#ifdef _DEBUG
	ai = (HANDLE)_beginthreadex(NULL,0,&aiUpdate,NULL,0,&threadID);
#endif
	NET_DATA data;

	for (;;)
	{
		//	��M
		int ret = recvfrom(recvSock, (char*)&data, sizeof(data), 0, (sockaddr*)&recvAdd, &recvAddLength);
		//ret = WSAGetLastError();
		//printf("%d", ret);]

		if (ret == SOCKET_ERROR)
		{
		}
		else
		{
			if (data.servID == SERV_ID)
			{
				//	�f�[�^�^�C�v�ɂ���ĕ���
				switch (data.type)
				{
				case DATA_TYPE_POS:

					//	�ʒu���̃Z�b�g
					userInfo[data.charNum].pos.x = data.data_pos.posX;
					userInfo[data.charNum].pos.y = data.data_pos.posY;
					userInfo[data.charNum].pos.z = data.data_pos.posZ;

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_ROT:

					//	��]���̃Z�b�g
					userInfo[data.charNum].rot.x = data.data_rot.rotX;
					userInfo[data.charNum].rot.y = data.data_rot.rotY;
					userInfo[data.charNum].rot.z = data.data_rot.rotZ;

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_CANNONROT:

					//	��]���̃Z�b�g
					userInfo[data.charNum].cannonRot.x = data.data_cannonRot.rotX;
					userInfo[data.charNum].cannonRot.y = data.data_cannonRot.rotY;
					userInfo[data.charNum].cannonRot.z = data.data_cannonRot.rotZ;

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_CANNON:

					//	���˃t���O�̃Z�b�g
					userInfo[data.charNum].cannon = data.data_cannon.flag;

					//	�}���`�L���X�g�ő��M�i���M��Ŏ����̃f�[�^�������珟��ɂ͂����j
					sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));

					break;

				case DATA_TYPE_KILL:

					//	�E�������C���N�������g
					userInfo[data.charNum].kill++;

					break;

				case DATA_TYPE_DEATH:

					//	�E���ꂽ���C���N�������g
					userInfo[data.charNum].death++;

					break;

				case DATA_TYPE_PAUSE:

					//	�|�[�Y���Ƀf�[�^��n��
					//	�w��̃L�����N�^�̂Ƃ���ɂ̂ݑ��M
					for (int count = 0; count < charcterMax; count++)
					{
						//	�����ȊO�̏ꍇ
						if (count != data.charNum)
						{
							data.charNum = count;

							//	���M�f�[�^�i�[
							data.servID = SERV_ID;
							data.type = DATA_TYPE_PAUSE;
							data.data_pause.kill = userInfo[count].kill;
							data.data_pause.death = userInfo[count].death;

							//	�f�[�^���M
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&userInfo[data.charNum].fromaddr, sizeof(userInfo[data.charNum].fromaddr));
						}
					}

					break;

				case DATA_TYPE_GAME_START:

					//	�ŏ�ʃN���C�A���g����A�Q�[���J�n���󂯎������
					if (data.charNum == 0)
					{
						// AI�����p�X���b�h�J�n
						ai = (HANDLE)_beginthreadex(NULL, 0, &aiUpdate, NULL, NULL, NULL);
					}

					break;

				case DATA_TYPE_ENTRY:

					//	�G���g���[����
					//	���݂̃v���C���[����Ԃ��A�����̃v���C���[�ԍ����Z�b�g������B
				{
					//	���[�v��������t���O
					bool errorFlag = true;

					//	�L�����N�^�[�ԍ����C���N�������g�i�L�����N�^�[�ԍ���0�`5�j
					charNum++;

					//	�L�����N�^�[�������l�𒴂��Ă����珈�����͂���
					if (charNum < charcterMax)
					{
						//	�G���[�����i�G���g���[��ԂȂ�G���[�Ƃ���j
						if (userInfo[charNum].entryFlag == true)
						{
							//	�L�����N�^�ԍ����}�C�i�X���A������e��
							charNum--;
							errorFlag = false;

							break;
						}

						//	�G���[�����𔲂�����
						if (errorFlag == true)
						{
							//	���l���Z�b�g
							data.charNum = charNum;
							data.servID = SERV_ID;
							data.type = DATA_TYPE_ENTRY;

							//	���[�U�[�����Z�b�g
							userInfo[charNum].fromaddr = recvAdd;
							userInfo[charNum].fromaddr.sin_port = htons(3000);
							userInfo[charNum].entryFlag = true;

							//	�u�G���g���[�����v�Ƃ��������}���`�L���X�g�ő��M
							sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&sendAdd, sizeof(sendAdd));
						}
					}
					else
					{
						data.type = DATA_TYPE_EMPTY;
						data.servID = SERV_ID;

						//	�|�[�g���Đݒ�
						recvAdd.sin_port = htons(3000);

						//	�����������ς��Ƒ���
						sendto(sendSock, (char*)&data, sizeof(data), 0, (sockaddr*)&recvAdd, sizeof(recvAdd));
					}

					break;
				}
				}
			}
			else
			{
				printf("networkID Not Equal!!\n");
			}
		}
	}

	CloseHandle(ai);
	// �\�P�b�g�I��
	closesocket(recvSock);

	// WinSock�I������
	WSACleanup();

	AI::Finalize();
}

//=============================================================================
//	AI�p�X�V�����֐�
//=============================================================================
unsigned __stdcall aiUpdate(void *p)
{
	unsigned int CurrentTime = 0;//DWORD dwCurrentTime;
	unsigned int PrevTime = 0;	 //DWORD dwExecLastTime;
	unsigned int FPSLastTime = 0;//DWORD dwFPSLastTime;
	unsigned int FrameCount = 0; //DWORD dwFrameCount;

	timeBeginPeriod(1);				// ����\��ݒ�

	FPSLastTime =
		CurrentTime = timeGetTime();

	PrevTime = 0;

	FrameCount = 0;

	//	�N���C�A���g����FPS�����낦��
	for (;;)
	{
		CurrentTime = timeGetTime();
		if ((CurrentTime - FPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
		{
			FPSLastTime = CurrentTime;
			FrameCount = 0;
		}

		if (CurrentTime - PrevTime >= 1000 / 60)
		{


			//	AI�X�V����
			AI::SetUserInfo(userInfo);
			AI::UpdateAll();


			PrevTime = CurrentTime;
		}
	}
}

//	EOF