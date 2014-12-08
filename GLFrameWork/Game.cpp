#include <math.h>

#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"
#include "Camera.h"
#include "PlayerCamera.h"

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 500.0f;   // ��̔��a
#include "CPlayer.h"

CPlayer* g_player;
CModel* g_model;


CGame::CGame()
{

}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	// �n�`����
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(100.0f, 100.0f), VECTOR2(2.0f, 2.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

#ifdef _DEBUG
	// �v���C���[����
	PlayerTest = CModel::Create(CModel::MIKU, VECTOR3(0.0f, 0.0f, 0.0f));
	PlayerTest->SetTex(CTexture::Texture(TEX_MIKU));
	PlayerTest->SetScl(20.0f, 20.0f, 20.0f);

	// �f�o�b�O���C���[�t���[��
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// �f�o�b�O�J�����ʒu
//	CCamera::Camera()->SetEye(0.0f, 100.0f, 0.0f);
#endif
}

void CGame::Uninit(void)
{
#ifdef _DEBUG
	// �v���C���[�j��
	if (PlayerTest != nullptr)
	{
		PlayerTest->Release();
		PlayerTest = nullptr;
	}
#endif

	// ��j��
	if (Sky != nullptr)
	{
		Sky->Release();
		Sky = nullptr;
	}

	// �n�`�j��
	if (Ground != nullptr)
	{
		Ground->Release();
		Ground = nullptr;
	}

	CObject::ReleaseAll();
}

void CGame::Update(void)
{
#ifdef _DEBUG
	// �v���C���[�ړ�
	if (CKeyboard::GetPress(DIK_W))
	{
		PlayerTest->AddPosZ(1.0f);
	}
	else if (CKeyboard::GetPress(DIK_S))
	{
		PlayerTest->AddPosZ(-1.0f);
	}
	if (CKeyboard::GetPress(DIK_A))
	{
		PlayerTest->AddPosX(1.0f);
	}
	else if (CKeyboard::GetPress(DIK_D))
	{
		PlayerTest->AddPosX(-1.0f);
	}

	// �n�`�Ƃ̂����蔻��
	VECTOR3	NormalGround;		// �n�`�̖@��
	float	HeightGround;		// �n�`�̍���
	HeightGround = Ground->GetHeight(PlayerTest->Pos(), &NormalGround);

	// ��]�����߂�
	VECTOR3	VectorUpPlayer;		// ������x�N�g��
	VECTOR3	VectorNormalYZ;		// YZ���ʏ�̖@���x�N�g��
	VECTOR3	VectorNormalXY;		// XY���ʏ�̖@���x�N�g��
	float	AnglePlayerX;		// �v���C���[��]X��
	float	AnglePlayerZ;		// �v���C���[��]Z��
	VectorUpPlayer.x = VectorUpPlayer.z = 0.0f;
	VectorUpPlayer.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglePlayerX = acosf(VECTOR3::Dot(VectorNormalYZ, VectorUpPlayer));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = acosf(VECTOR3::Dot(VectorNormalXY, VectorUpPlayer));

	// �v���C���[�ɐݒ肷��
	PlayerTest->SetPosY(HeightGround);
	PlayerTest->SetRotX(180.0f / PI * AnglePlayerX);
	PlayerTest->SetRotZ( 180.0f / PI * AnglePlayerZ);
#endif

	// �V�[���J��
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}