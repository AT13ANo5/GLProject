//------------------------------------------------------------------------------
//
//  []
// Author : 
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------------------------
#include <math.h>
#include <float.h>

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
#include "CPlayer.h"
#include "CBullet.h"
#include "PlayerCamera.h"


#include "UI.h"

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 1500.0f;   // ��̔��a
CPlayer* Player = nullptr;//�v���C���[
const float	CGame::RADIUS_DEFENSE_CHARACTER = 10.0f;	// �L�����N�^�[�̖h�䔼�a
const float	CGame::HEIGHT_DEFENSE_CHARACTER = 10.0f;	// �L�����N�^�[�̖h�䒆�S����
const float	CGame::RADIUS_OFFENSE_BULLET = 10.0f;		// �C�e�̍U�����a
const float	CGame::HEIGHT_OFFENSE_BULLET = 10.0f;		// �C�e�̍U�����S����
const float	CGame::RADIUS_PUSH_CHARACTER = 10.0f;		// �L�����N�^�[�̉����߂����a
const float	CGame::HEIGHT_PUSH_CHARACTER = 10.0f;		// �L�����N�^�[�̉����߂����S����
const float	CGame::RADIUS_DEFENSE_ROCK = 10.0f;			// ��̖h�䔼�a
const float	CGame::HEIGHT_DEFENSE_ROCK = 10.0f;			// ��̖h�䒆�S����
const float	CGame::RADIUS_PUSH_ROCK = 10.0f;			// ��̉����߂����a
const float	CGame::HEIGHT_PUSH_ROCK = 10.0f;			// ��̉����߂����S����

const int	CGame::MAX_ROCK = 100;						// ��̐�

// �萔
namespace{

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
CGame::CGame()
{

	UI = nullptr;
	ppRock_ = nullptr;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
CGame::~CGame()
{

}

//------------------------------------------------------------------------------
// �Q�[���̏�����
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CGame::Init(void)
{
	//CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	//polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	//polygon->SetColor(GREEN(1.0f));
	CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));	// �n�`����

	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(100.0f,100.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));
#ifdef _DEBUG
	// �f�o�b�O���C���[�t���[��
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// �v���C���[����
	Player = CPlayer::Create(CModel::RINCHAN, VECTOR3(0.0f, 30.0f, 0.0f), 0);
	Player->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));
	Player->SetRot(0.0f,180.0f,0.0f);

	//�v���C���[�J��������
	CPlayerCamera::Create(Player,300.0f);

	// UI������
	UI = new CUI;
	UI->Init();
  UI->SetPlayer(Player);

	// ��̐���
	ppRock_ = new CModel*[MAX_ROCK];
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		VECTOR3	positionRock(0.1f * ((rand() % 10000) - 5000), 0.0f, 0.1f * ((rand() % 10000) - 5000));
		ppRock_[cntRock] = CModel::Create(CModel::ROCK, positionRock);
		PushBackObjectByField(ppRock_[cntRock]);
	}

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// ��̔j��
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		SafeRelease(ppRock_[cntRock]);
	}
	SafeDeletes(ppRock_);
	// �v���C���[�j��
	SafeRelease(Player);

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

	// UI�j��
  SafeDelete(UI);

	CCamera::ReleaseAll();
	CObject::ReleaseAll();
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CGame::Update(void)
{
	// �n�`�Ƃ̂����蔻��
	VECTOR3	NormalGround;		// �n�`�̖@��
	float	HeightGround;		// �n�`�̍���
	HeightGround = Ground->GetHeight(Player->Pos(),&NormalGround);

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
	AnglePlayerX = -acosf(VECTOR3::Dot(VectorNormalYZ,VectorUpPlayer));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = -acosf(VECTOR3::Dot(VectorNormalXY,VectorUpPlayer));

	// �v���C���[���̃f�o�b�O�\��
	VECTOR3	positionPlayer = Player->Pos();
	VECTOR3	rotaionPlayer = Player->Rot();
	Console::SetCursorPos(1,1);
	Console::Print("Pos : (%9.3f, %9.3f, %9.3f)",positionPlayer.x,HeightGround,positionPlayer.z);
	Console::Print("Rot : (%9.3f, %9.3f, %9.3f)",180.0f / PI * AnglePlayerX,rotaionPlayer.y,180.0f / PI * AnglePlayerZ);

	// �v���C���[�ɐݒ肷��
	Player->SetPosY(HeightGround);
	Player->SetRotX(AnglePlayerX * 180.0f / PI);
	Player->SetRotZ(AnglePlayerZ * 180.0f / PI);

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}


	// �U������
	CheckHit();

	// �L�����N�^�[���m�̉����߂�
	PushBackCharacter();

	// �L�����N�^�[�Ɗ�̉����߂�
	PushBackRock();

	// �n�`�Ƃ̉����߂�
	PushBackField();

  // UI�̃A�b�v�f�[�g
  UI->Update();
}

//==============================================================================
// �U������̓����蔻��
//==============================================================================
void CGame::CheckHit(void)
{
	// �U�����̌���
	CPlayer*	pPlayerOffense = nullptr;		// �U�����v���C���[
	int			numPlayer = 1;					// �v���C���[��
	for (int cntBullet = 0; cntBullet < numPlayer; ++cntBullet)
	{
		// �v���C���[���擾
		pPlayerOffense = Player;

		// �e�����݂��Ȃ���Δ��肵�Ȃ�
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntPlayer = 0; cntPlayer < numPlayer; ++cntPlayer)
		{
			// �v���C���[���擾
			CPlayer*	pPlayerDefense = nullptr;		// �h�䑤�v���C���[

			// �v���C���[������\���m�F
			if (NeedsSkipPlayer(pPlayerDefense))
			{
				continue;
			}
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// �����蔻��
			VECTOR3	positionOffense = pPlayerOffense->Bullet()->Pos();		// �U�����蒆�S���W
			VECTOR3	positionDefense = pPlayerDefense->Pos();				// �h�䔻�蒆�S���W
			VECTOR3	vectorOffenseToDefense;									// �U�����肩��h�䔻��ւ̃x�N�g��
			float	distanceOffenseAndDefense;								// ����̒��S���m�̋���
			positionOffense.y += HEIGHT_OFFENSE_BULLET;
			positionDefense.y += HEIGHT_DEFENSE_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z;
			if (distanceOffenseAndDefense < (RADIUS_DEFENSE_CHARACTER + RADIUS_OFFENSE_BULLET) * (RADIUS_DEFENSE_CHARACTER + RADIUS_OFFENSE_BULLET))
			{
				// ���������Ƃ��̏���
			}
		}
	}
}

//==============================================================================
// �L�����N�^�[���m�̉����߂�
//==============================================================================
void CGame::PushBackCharacter(void)
{
	// �U�����̌���
	CPlayer*	pPlayerOffense = nullptr;		// �U�����v���C���[
	int			numPlayer = 1;					// �v���C���[��
	for (int cntBullet = 0; cntBullet < numPlayer; ++cntBullet)
	{
		// �v���C���[���擾
		pPlayerOffense = Player;

		// �v���C���[������\���m�F
		if (NeedsSkipPlayer(pPlayerOffense))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntPlayer = 0; cntPlayer < numPlayer; ++cntPlayer)
		{
			// �v���C���[���擾
			CPlayer*	pPlayerDefense = nullptr;		// �h�䑤�v���C���[

			// �v���C���[������\���m�F
			if (NeedsSkipPlayer(pPlayerDefense))
			{
				continue;
			}
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// �����蔻��
			VECTOR3	positionOffense = pPlayerOffense->Pos();		// �U�����蒆�S���W
			VECTOR3	positionDefense = pPlayerDefense->Pos();		// �h�䔻�蒆�S���W
			VECTOR3	vectorOffenseToDefense;							// �U�����肩��h�䔻��ւ̃x�N�g��
			float	distanceOffenseAndDefense;						// ����̒��S���m�̋���
			positionOffense.y += HEIGHT_PUSH_CHARACTER;
			positionDefense.y += HEIGHT_PUSH_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z );
			if (distanceOffenseAndDefense < 2.0f * RADIUS_PUSH_CHARACTER)
			{
				// �����߂�
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * 0.51f * (2.0f * RADIUS_PUSH_CHARACTER - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					pPlayerOffense->AddPos(vectorPushBack);
					vectorPushBack *= -1.0f;
					pPlayerDefense->AddPos(vectorPushBack);
				}
				else
				{
					pPlayerOffense->AddPosX(RADIUS_PUSH_CHARACTER);
					pPlayerDefense->AddPosX(-RADIUS_PUSH_CHARACTER);
				}
			}
		}
	}
}

//==============================================================================
// �L�����N�^�[�Ɗ�̉����߂�
//==============================================================================
void CGame::PushBackRock(void)
{
	// �U�����̌���
	CPlayer*	pPlayer = nullptr;		// �U�����v���C���[
	int			numPlayer = 1;			// �v���C���[��
	for (int cntPlayer = 0; cntPlayer < numPlayer; ++cntPlayer)
	{
		// �v���C���[���擾
		pPlayer = Player;

		// �v���C���[������\���m�F
		if (NeedsSkipPlayer(pPlayer))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// ����擾
			CObject*	pRock = ppRock_[cntRock ];		// ��I�u�W�F�N�g

			// �����蔻��
			VECTOR3	positionPlayer = pPlayer->Pos();	// �U�����蒆�S���W
			VECTOR3	positionRock = pRock->Pos();		// �h�䔻�蒆�S���W
			VECTOR3	vectorOffenseToDefense;				// �U�����肩��h�䔻��ւ̃x�N�g��
			float	distanceOffenseAndDefense;			// ����̒��S���m�̋���
			positionPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positionPlayer;
			distanceOffenseAndDefense = sqrtf( vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z );
			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + HEIGHT_PUSH_ROCK)
			{
				// �����߂�
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * -(RADIUS_PUSH_CHARACTER + HEIGHT_PUSH_ROCK - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					vectorPushBack.y = 0.0f;
					pPlayer->AddPos(vectorPushBack);
				}
				else
				{
					pPlayer->AddPosX(RADIUS_PUSH_CHARACTER + HEIGHT_PUSH_ROCK);
				}
			}
		}
	}
}

//==============================================================================
// �n�`�̉����߂�
//==============================================================================
void CGame::PushBackField(void)
{
	// ����
	CPlayer*	pPlayerCurrent = nullptr;		// �ΏۃI�u�W�F�N�g
	int			numObject = 1;					// �ΏۃI�u�W�F�N�g��
	for (int cntPlayer = 0; cntPlayer < numObject; ++cntPlayer)
	{
		// �ΏۃI�u�W�F�N�g���擾
		pPlayerCurrent = Player;

		// �Ώۂ̃X�e�[�g���m�F
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// �����߂�
		PushBackObjectByField(pPlayerCurrent);
	}
}

//==============================================================================
// �I�u�W�F�N�g�̒n�`�ɂ�鉟���߂�
//==============================================================================
void CGame::PushBackObjectByField(CObject* pObject)
{
	// �n�`�Ƃ̂����蔻��
	VECTOR3	NormalGround;		// �n�`�̖@��
	float	HeightGround;		// �n�`�̍���
	HeightGround = Ground->GetHeight(pObject->Pos(), &NormalGround);

	// ��]�����߂�
	VECTOR3	VectorUppObject;		// ������x�N�g��
	VECTOR3	VectorNormalYZ;			// YZ���ʏ�̖@���x�N�g��
	VECTOR3	VectorNormalXY;			// XY���ʏ�̖@���x�N�g��
	float	AnglepObjectX;			// �v���C���[��]X��
	float	AnglepObjectZ;			// �v���C���[��]Z��
	VectorUppObject.x = VectorUppObject.z = 0.0f;
	VectorUppObject.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglepObjectX = -acosf(VECTOR3::Dot(VectorNormalYZ, VectorUppObject));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglepObjectZ = -acosf(VECTOR3::Dot(VectorNormalXY, VectorUppObject));

	// �L�����N�^�[�ɐݒ肷��
	pObject->SetPosY(HeightGround);
	//	pObject->SetRotX(AngleObjectX * 180.0f / PI);
	//	pObject->SetRotZ(AngleObjectZ * 180.0f / PI);
}

//==============================================================================
// �v���C���[����X�L�b�v
//==============================================================================
bool CGame::NeedsSkipPlayer(CPlayer* pPlayer)
{
	// �X�e�[�g���m�F
	int		statePlayer = 0;// pPlayerCurrent->State();
	if (statePlayer == PLAYER_STATE_DEATH || statePlayer == PLAYER_STATE_RESPAWN)
	{
		return true;
	}

	// �X�L�b�v���Ȃ�
	return false;
}

//==============================================================================
// �C�e����X�L�b�v
//==============================================================================
bool CGame::NeedsSkipBullet(CPlayer* pPlayer)
{
	// �e�����݂��Ȃ��Ƃ�
//	if (!pPlayer->ExistsBullet())
//	{
//		return true;
//	}

	// �X�L�b�v���Ȃ�
	return false;
}

//------------------------------------------------------------------------------
// EOF
