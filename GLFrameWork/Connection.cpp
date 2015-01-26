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

//*****************************************************************************
//	�萔��`
//*****************************************************************************
static const int connectionPlayerMax = 6;	//	�v���C���[�ő吔
static const float windowWidth = 206.0f;	//	�摜�I�u�W�F�N�g�̉���
static const float windowHeight = 103.0f;	//	�摜�I�u�W�F�N�g�̏c��
static const float waitPlayerWidth = 80.0f;	//	�摜�I�u�W�F�N�g�̉���
static const float waitPlayerHeight = 80.0f;//	�摜�I�u�W�F�N�g�̏c��
static const float waitStringWidth = 80.0f;	//	�摜�I�u�W�F�N�g�̉���
static const float waitStringHeight = 20.0f;//	�摜�I�u�W�F�N�g�̏c��

static const float windowHeightBase = SCREEN_HEIGHT / 2.0f - 150;	//	�摜�I�u�W�F�N�g�z�u�̊�ʒu�i���j
static const float windowMarginWidth = 50.0f;	//	�摜�I�u�W�F�N�g�z�u�̊�
static const float windowMarginHeight = 51.0f;		//	�摜�I�u�W�F�N�g�z�u�̊�

//	�摜�I�u�W�F�N�g�z�u�̊�ʒu
static const VECTOR3 windowPos[connectionPlayerMax] =
{
	VECTOR3(windowWidth + windowMarginWidth, windowHeightBase + 180.0f, 0),
	VECTOR3(windowPos[0].x + windowWidth * 1.5f + windowMarginWidth, windowHeightBase + windowMarginHeight + windowHeight * 0.5f, 0),
	VECTOR3(windowPos[1].x + windowWidth + windowMarginWidth, windowPos[1].y, 0),
	VECTOR3(windowPos[2].x + windowWidth + windowMarginWidth, windowPos[1].y, 0),
	VECTOR3(windowPos[0].x + windowWidth * 2.0f + windowMarginWidth * 1.5f, windowPos[1].y + windowMarginHeight + windowHeight, 0),
	VECTOR3(windowPos[4].x + windowWidth + windowMarginWidth, windowPos[4].y, 0)
};

//	�摜�I�u�W�F�N�g�z�u�̊�ʒu
static const VECTOR3 waitPlayerPos[connectionPlayerMax] =
{
	VECTOR3(windowPos[0].x, windowPos[0].y - 30.0f, 0),
	VECTOR3(windowPos[1].x, windowPos[1].y - 30.0f, 0),
	VECTOR3(windowPos[2].x, windowPos[2].y - 30.0f, 0),
	VECTOR3(windowPos[3].x, windowPos[3].y - 30.0f, 0),
	VECTOR3(windowPos[4].x, windowPos[4].y - 30.0f, 0),
	VECTOR3(windowPos[5].x, windowPos[5].y - 30.0f, 0)
};

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
	backGround = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0), VECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	backGround->SetTex(CTexture::Texture(TEX_FIELD));
	//-------------------------------------------------

	//	�����I�u�W�F�N�g�u���S�v�̔z�u
	//-------------------------------------------------
	Logo = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f, 100.0f, 0), VECTOR2(512.0f, 200.0f));
	Logo->SetTex(CTexture::Texture(TEX_MIKU));
	//-------------------------------------------------

	//	�����I�u�W�F�N�g�uPUSHSTART�v�̔z�u
	//-------------------------------------------------
	pushStart = CPushStart::Create(VECTOR3(SCREEN_WIDTH / 2.0f, 650.0f, 0), VECTOR2(512.0f, 100.0f));
	pushStart->SetTex(CTexture::Texture(TEX_CONNECTION));
	//-------------------------------------------------

	//	�T�[�o�[�փG���g���[���𑗐M
	CManager::SendEntry();

	//	�摜�I�u�W�F�N�g�Q�̔z�u
	//-------------------------------------------------
	{
		waitPlayer = new CPolygon2D*[connectionPlayerMax];

		waitPlayer[0] = CPolygon2D::Create(waitPlayerPos[0], VECTOR2(waitPlayerWidth * 2.0f, waitPlayerHeight * 2.0f));
		waitPlayer[0]->SetTex(CTexture::Texture(TEX_MIKU));

		waitPlayer[1] = CPolygon2D::Create(waitPlayerPos[1], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[1]->SetTex(CTexture::Texture(TEX_MIKU));

		waitPlayer[2] = CPolygon2D::Create(waitPlayerPos[2], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[2]->SetTex(CTexture::Texture(TEX_MIKU));

		waitPlayer[3] = CPolygon2D::Create(waitPlayerPos[3], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[3]->SetTex(CTexture::Texture(TEX_MIKU));

		waitPlayer[4] = CPolygon2D::Create(waitPlayerPos[4], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[4]->SetTex(CTexture::Texture(TEX_MIKU));

		waitPlayer[5] = CPolygon2D::Create(waitPlayerPos[5], VECTOR2(waitPlayerWidth, waitPlayerHeight));
		waitPlayer[5]->SetTex(CTexture::Texture(TEX_MIKU));
	}
	//-------------------------------------------------

	//	�摜�I�u�W�F�N�g�Q�̔z�u
	//-------------------------------------------------
	{
		waitBackGround = new CPolygon2D*[connectionPlayerMax];
		waitBackGround[0] = CPolygon2D::Create(windowPos[0], VECTOR2(windowWidth * 2.0f, windowHeight * 2.0f));
		waitBackGround[0]->SetTex(CTexture::Texture(TEX_TEST));

		waitBackGround[1] = CPolygon2D::Create(windowPos[1], VECTOR2(windowWidth, windowHeight));
		waitBackGround[1]->SetTex(CTexture::Texture(TEX_TEST));

		waitBackGround[2] = CPolygon2D::Create(windowPos[2], VECTOR2(windowWidth, windowHeight));
		waitBackGround[2]->SetTex(CTexture::Texture(TEX_TEST));

		waitBackGround[3] = CPolygon2D::Create(windowPos[3], VECTOR2(windowWidth, windowHeight));
		waitBackGround[3]->SetTex(CTexture::Texture(TEX_TEST));

		waitBackGround[4] = CPolygon2D::Create(windowPos[4], VECTOR2(windowWidth, windowHeight));
		waitBackGround[4]->SetTex(CTexture::Texture(TEX_TEST));

		waitBackGround[5] = CPolygon2D::Create(windowPos[5], VECTOR2(windowWidth, windowHeight));
		waitBackGround[5]->SetTex(CTexture::Texture(TEX_TEST));
	}
	//-------------------------------------------------
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
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_GAME);
	}
}

//	EOF