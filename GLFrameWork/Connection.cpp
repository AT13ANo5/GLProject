//=============================================================================
//	Connection�t�@�C�� [ Connection.h ]
//	Auther : KOTARO NAGASAKI
//=============================================================================

//*****************************************************************************
//	�C���N���[�h
//*****************************************************************************
#include "Connection.h"
#include "Polygon2D.h"
#include "CPushStart.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "netData.h"
#include "ManagerGL.h"
#include "SoundAL.h"

//*****************************************************************************
//	�萔��`
//*****************************************************************************
static const int connectionPlayerMax = 6;	//	�v���C���[�ő吔
static const float windowWidth = 256.0f;	//	�摜�I�u�W�F�N�g�̉���
static const float windowHeight = 231.0f;	//	�摜�I�u�W�F�N�g�̏c��
static const float waitPlayerWidth = 256.0f;	//	�摜�I�u�W�F�N�g�̉���
static const float waitPlayerHeight = 231.0f;	//	�摜�I�u�W�F�N�g�̏c��

//	�摜�I�u�W�F�N�g�z�u�̊�ʒu
static const VECTOR3 windowPos[connectionPlayerMax] =
{
	VECTOR3(270.0f, 410.0f, 0),
	VECTOR3(755.0f, 300.0f, 0),
	VECTOR3(1040.0f, 300.0f, 0),
	VECTOR3(635.0f, 505.0f, 0),
	VECTOR3(885.0f, 505.0f, 0),
	VECTOR3(1130.0f, 505.0f, 0),
};

//	�摜�I�u�W�F�N�g�z�u�̊�ʒu
static const VECTOR3 waitPlayerPos[connectionPlayerMax] =
{
	windowPos[0],
	windowPos[1],
	windowPos[2],
	windowPos[3],
	windowPos[4],
	windowPos[5],
};

CPolygon2D* CConnection::Logo;
CPolygon2D* CConnection::backGround;
CPolygon2D** CConnection::waitBackGround;
CPolygon2D** CConnection::waitPlayer;
CPushStart* CConnection::pushStart;

//*****************************************************************************
//	�ϐ���`
//*****************************************************************************

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CConnection::CConnection()
{
	Logo = nullptr;
	pushStart = nullptr;
	backGround = nullptr;
}

//=============================================================================
//	�f�X�g���N�^
//=============================================================================
CConnection::~CConnection()
{
}

//=============================================================================
//	����������
//=============================================================================
void CConnection::Init(void)
{
	//	�w�i�摜�̔z�u
	//-------------------------------------------------
	backGround = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0),
		VECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	backGround->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_BG));
	//-------------------------------------------------

	//	�����I�u�W�F�N�g�u���S�v�̔z�u
	//-------------------------------------------------
	Logo = CPolygon2D::Create(VECTOR3(670.0f, 90.0f, 0),
		VECTOR2(750.0f, 125.0f));
	Logo->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_LOGO));
	//-------------------------------------------------

	//	�����I�u�W�F�N�g�uPUSHSTART�v�̔z�u
	//-------------------------------------------------
	pushStart = CPushStart::Create(VECTOR3(655.0f, 690.0f, 0),
		VECTOR2(325.0f, 50.0f));
	pushStart->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_ENTER));
	//-------------------------------------------------

	//	�T�[�o�[�փG���g���[���𑗐M
	CManager::SendEntry();

	//	�摜�I�u�W�F�N�g�Q�̔z�u
	//-------------------------------------------------
	{
		waitPlayer = new CPolygon2D*[connectionPlayerMax];

		waitPlayer[0] = CPolygon2D::Create(waitPlayerPos[0], VECTOR2(waitPlayerWidth * 2.0f, waitPlayerHeight * 2.0f));
		waitPlayer[0]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_RED));

		waitPlayer[1] = CPolygon2D::Create(waitPlayerPos[1], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[1]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_BLUE));

		waitPlayer[2] = CPolygon2D::Create(waitPlayerPos[2], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[2]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_WATER));

		waitPlayer[3] = CPolygon2D::Create(waitPlayerPos[3], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[3]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_ORANGE));

		waitPlayer[4] = CPolygon2D::Create(waitPlayerPos[4], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[4]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_WHITE));

		waitPlayer[5] = CPolygon2D::Create(waitPlayerPos[5], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[5]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_RINCHAN));
	}
	//-------------------------------------------------

	//	�摜�I�u�W�F�N�g�Q�̔z�u
	//-------------------------------------------------
	{
		waitBackGround = new CPolygon2D*[connectionPlayerMax];
		waitBackGround[0] = CPolygon2D::Create(windowPos[0],VECTOR2(windowWidth * 2.0f, windowHeight * 2.0f));
		waitBackGround[0]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_CPU));

		waitBackGround[1] = CPolygon2D::Create(windowPos[1], VECTOR2(windowWidth, windowHeight));
		waitBackGround[1]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_CPU));

		waitBackGround[2] = CPolygon2D::Create(windowPos[2], VECTOR2(windowWidth, windowHeight));
		waitBackGround[2]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_CPU));

		waitBackGround[3] = CPolygon2D::Create(windowPos[3], VECTOR2(windowWidth, windowHeight));
		waitBackGround[3]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_CPU));

		waitBackGround[4] = CPolygon2D::Create(windowPos[4], VECTOR2(windowWidth, windowHeight));
		waitBackGround[4]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_CPU));

		waitBackGround[5] = CPolygon2D::Create(windowPos[5], VECTOR2(windowWidth, windowHeight));
		waitBackGround[5]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_CPU));
	}
	//-------------------------------------------------
	CSoundAL::Play(CSoundAL::BGM_CONNECT);
}

//=============================================================================
//	�I������
//=============================================================================
void CConnection::Uninit(void)
{
	if (pushStart != nullptr)
	{
		pushStart->Release();
		pushStart = nullptr;
	}

	if (Logo != nullptr)
	{
		Logo->Release();
		Logo = nullptr;
	}

	for (int count = 0; count < connectionPlayerMax; count++)
	{
		if (waitBackGround[count] != nullptr)
		{
			waitBackGround[count]->Release();
			waitBackGround[count] = nullptr;
		}

		if (waitPlayer[count] != nullptr)
		{
			waitPlayer[count]->Release();
			waitPlayer[count] = nullptr;
		}
	}

	if (waitPlayer != nullptr)
	{
		delete[] waitPlayer;
		waitPlayer = nullptr;
	}

	if (waitBackGround != nullptr)
	{
		delete[] waitBackGround;
		waitBackGround = nullptr;
	}

	if (backGround != nullptr)
	{
		backGround->Release();
		backGround = nullptr;
	}
}
//=============================================================================
//	�X�V����
//=============================================================================
void CConnection::Update(void)
{
	recvUpdate();
	keyUpdate();
}
//=============================================================================
//	�T�[�o�[����̎�M�󋵂ɂ��X�V����
//=============================================================================
void CConnection::recvUpdate()
{
	//	�l�b�g���[�N�֘A�̃f�[�^���擾
	NETWORK_DATA* netWorkData = CManager::getNetWorkData();

	//	�����������ς��Ȃ�
	if (netWorkData->emptyFlag == true)
	{
		//	��������t
	}
}
//=============================================================================
//	�L�[�����̂��X�V����
//=============================================================================
void CConnection::keyUpdate()
{
	//	�������e�Ȃ�
	if (CManager::netData.charNum == 0)
	{
		//	�V�[���؂�ւ�
		if (CKeyboard::GetTrigger(DIK_RETURN))
		{
			CSoundAL::Play(CSoundAL::SE_ENTER);
			CManager::SendChangeGame();
			CManager::ChangeScene(SCENE_GAME);
		}
	}
}
//=============================================================================
//	�uCPU�v����G���g���[�ւ̕ύX����
//=============================================================================
void CConnection::setEntry(int _charNum)
{
	waitBackGround[_charNum]->SetTex(CTexture::Texture(TEX_NETWARK_YOUJO_READY));
}

//	EOF