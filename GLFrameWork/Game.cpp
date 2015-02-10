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
#include "BattleAreaCylinder.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "PlayerCamera.h"
#include "MiniMap.h"

#include "UI.h"

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 1500.0f;   // ��̔��a
CPlayer** CGame::Player;
CPlayer* Player = nullptr;//�v���C���[
const float	CGame::RADIUS_DEFENSE_CHARACTER = 10.0f;	// �L�����N�^�[�̖h�䔼�a
const float	CGame::HEIGHT_DEFENSE_CHARACTER = 10.0f;	// �L�����N�^�[�̖h�䒆�S����
const float	CGame::RADIUS_OFFENSE_BULLET = 10.0f;		// �C�e�̍U�����a
const float	CGame::HEIGHT_OFFENSE_BULLET = 10.0f;		// �C�e�̍U�����S����
const float	CGame::RADIUS_PUSH_CHARACTER = 10.0f;		// �L�����N�^�[�̉����߂����a
const float	CGame::HEIGHT_PUSH_CHARACTER = 10.0f;		// �L�����N�^�[�̉����߂����S����
const float	CGame::RADIUS_DEFENSE_ROCK = 45.0f;			// ��̖h�䔼�a
const float	CGame::HEIGHT_DEFENSE_ROCK = 45.0f;			// ��̖h�䒆�S����
const float	CGame::RADIUS_PUSH_ROCK = 45.0f;			// ��̉����߂����a
const float	CGame::HEIGHT_PUSH_ROCK = 45.0f;			// ��̉����߂����S����
const float CGame::FIELD_PANEL_SIZE = 35.0f;			//�t�B�[���h�̃p�l���ꖇ�̃T�C�Y

const float	CGame::RADIUS_AREA_BATTLE = 1000.0f;		// �퓬�G���A���a
const float	CGame::HEIGHT_WALL = 500.0f;				// �ǂ̍���

const int	CGame::MAX_ROCK = 20;						// ��̐�

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
	//CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));	// �n�`����

	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(FIELD_PANEL_SIZE,FIELD_PANEL_SIZE),VECTOR2(0,0),1.5f);
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	// ���E������
	CylinderArea = nullptr;
	CylinderArea = CBattleAreaCylinder::Create(VECTOR3(0.0f, 0.0f, 0.0f), HEIGHT_WALL, VECTOR2(64.0f, 1.0f), RADIUS_AREA_BATTLE, VECTOR2(1, -0.5f));
	CylinderArea->SetTex(CTexture::Texture(TEX_WALL));
	CylinderArea->SetAlpha(0.5f);

#ifdef _DEBUG
	// �f�o�b�O���C���[�t���[��
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	Player = new CPlayer*[PLAYER_MAX];
	// �v���C���[����
	for(int i = 0; i < PLAYER_MAX; i++)
	{
		Player[i] = CPlayer::Create(CModel::RINCHAN, VECTOR3(0.0f + i * 50.0f, 30.0f, 0.0f), 0);
		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));
		Player[i]->SetRot(0.0f,180.0f,0.0f);

		if(i == CManager::netData.charNum)
		{
			Player[i]->SetPlayerFlag(true);
		}
	}	//�v���C���[�J��������
	CPlayerCamera::Create(Player[CManager::netData.charNum], 300.0f);

	// UI������
	UI = new CUI;
	UI->Init();
	UI->SetPlayer(Player);
	UI->MiniMap()->SetFieldSize(Ground->Size());

	// ��̐���
	ppRock_ = new CModel*[MAX_ROCK];
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		VECTOR3	positionRock(0.1f * ((rand() % 10000) - 5000), 100.0f, 0.1f * ((rand() % 10000) - 5000));
		ppRock_[cntRock] = CModel::Create(CModel::ROCK, positionRock);
		ppRock_[cntRock]->SetScl(1,1,1);
  ppRock_[cntRock]->SetTex(CTexture::Texture(TEX_ROCK));
		PushBackObjectByField(ppRock_[cntRock]);
	}

	CManager::gameStartFlag = true;
}
void CGame::SetPlayerState(NET_DATA _netData, DATA_TYPE _dataType)
{
	if (_netData.charNum != CManager::netData.charNum)
	{
		switch (_dataType)
		{
		case DATA_TYPE_POS:

			Player[_netData.charNum]->SetPosX(_netData.data_pos.posX);
			Player[_netData.charNum]->SetPosY(_netData.data_pos.posY);
			Player[_netData.charNum]->SetPosZ(_netData.data_pos.posZ);

			break;

		case DATA_TYPE_ROT:

			Player[_netData.charNum]->SetRotX(_netData.data_rot.rotX);
			Player[_netData.charNum]->SetRotY(_netData.data_rot.rotY);
			Player[_netData.charNum]->SetRotZ(_netData.data_rot.rotZ);

			break;

		case DATA_TYPE_CANNON:

			break;
		}
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
	CManager::gameStartFlag = false;

	// ��̔j��
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		SafeRelease(ppRock_[cntRock]);
		ppRock_[cntRock] = nullptr;
	}
	SafeDeletes(ppRock_);

	// �v���C���[�j��
	//SafeRelease(Player);
	for(int i = 0; i < PLAYER_MAX; i++)
	{
		SafeRelease(Player[i]);
	}

	SafeDeletes(Player);

	// ���E���̔j��
	SafeRelease(CylinderArea);

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
	UI->Uninit();
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
	// ���U�Q�[�W
	const float currentTimer = (float)Player[0]->ReloadTimer();
	const float maxTimer = (float)PLAYER_RELOAD_TIME;
	const float rate = currentTimer / maxTimer;	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}


	// �U������
	CheckHitPlayer();

	// �U������
	CheckHitRock();

	// �L�����N�^�[���m�̉����߂�
	PushBackCharacter();

	// �L�����N�^�[�Ɗ�̉����߂�
	PushBackRock();

	// �n�`�Ƃ̉����߂�
	PushBackField();

	// �n�`�Ƃ̔���
	IsLandField();

	// �s���\�͈͔���
	PushBackBattleArea();

	// UI�̃A�b�v�f�[�g
	UI->Update();

}

//==============================================================================
// �U������̓����蔻�� �v���C���[
//==============================================================================
void CGame::CheckHitPlayer(void)
{
	// �U�����̌���
	CPlayer*	pPlayerOffense = nullptr;		// �U�����v���C���[
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// �v���C���[���擾
		pPlayerOffense = Player[cntBullet];

		// �e�����݂��Ȃ���Δ��肵�Ȃ�
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
		{
			// ���g�Ƃ͔��肵�Ȃ�
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// �v���C���[���擾
			CPlayer*	pPlayerDefense = Player[cntPlayer];		// �h�䑤�v���C���[

			// �v���C���[������\���m�F
			if (NeedsSkipPlayer(pPlayerDefense))
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
				// �e�̏���
				pPlayerOffense->ReleaseBullet();

				// ���������Ƃ��̏���
				pPlayerDefense->SetState(PLAYER_STATE_DAMAGE);
				pPlayerDefense->AddPlayerLife(-1);

				// �G�t�F�N�g�F�����@�e���v���C���[�ɓ��������Ƃ�

				// �����I��
				break;
			}
		}
	}
}

//==============================================================================
// �U������̓����蔻�� ��
//==============================================================================
void CGame::CheckHitRock(void)
{
	// �U�����̌���
	CPlayer*	pPlayerOffense = nullptr;		// �U�����v���C���[
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// �v���C���[���擾
		pPlayerOffense = Player[cntBullet];

		// �e�����݂��Ȃ���Δ��肵�Ȃ�
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// �v���C���[���擾
			CObject*	pRock = ppRock_[cntRock];		// �h�䑤��

			// �����蔻��
			VECTOR3	positionOffense = pPlayerOffense->Bullet()->Pos();		// �U�����蒆�S���W
			VECTOR3	positionDefense = pRock->Pos();							// �h�䔻�蒆�S���W
			VECTOR3	vectorOffenseToDefense;									// �U�����肩��h�䔻��ւ̃x�N�g��
			float	distanceOffenseAndDefense;								// ����̒��S���m�̋���
			float	scalingRock;											// ��̑傫��
			scalingRock = (pRock->Scl().x < pRock->Scl().z ? pRock->Scl().x : pRock->Scl().z);
			positionOffense.y += HEIGHT_OFFENSE_BULLET;
			positionDefense.y += HEIGHT_DEFENSE_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z;

			if (distanceOffenseAndDefense < (RADIUS_DEFENSE_ROCK * scalingRock + RADIUS_OFFENSE_BULLET) * (RADIUS_DEFENSE_ROCK * scalingRock + RADIUS_OFFENSE_BULLET))
			{
				// �e�̏���
				pPlayerOffense->ReleaseBullet();

				// �G�t�F�N�g�F�����@�e���v���C���[�ɓ��������Ƃ�

				// �����I��
				break;
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
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// �v���C���[���擾
		pPlayerOffense = Player[cntBullet];

		// �v���C���[������\���m�F
		if (NeedsSkipPlayer(pPlayerOffense))
		{
			continue;
		}

		// �h�䑤�̌���
		for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
		{
			// �v���C���[���擾
			CPlayer*	pPlayerDefense = Player[ cntPlayer];		// �h�䑤�v���C���[

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
					pPlayerDefense->AddPos(vectorPushBack);
					vectorPushBack *= -1.0f;
					pPlayerOffense->AddPos(vectorPushBack);
				}
				else
				{
					pPlayerOffense->AddPosX(RADIUS_PUSH_CHARACTER);
					pPlayerDefense->AddPosX(-RADIUS_PUSH_CHARACTER);
				}

				// �G�t�F�N�g�F�Ήԁ@�v���C���[���m�̂Ԃ���
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
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// �v���C���[���擾
		pPlayer = Player[cntPlayer];

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
			float	scalingRock;						// ��̑傫��
			scalingRock = (pRock->Scl().x < pRock->Scl().z ? pRock->Scl().x : pRock->Scl().z);
			positionPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positionPlayer;
			distanceOffenseAndDefense = sqrtf( vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z );
			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock)
			{
				// �����߂�
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * -(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					vectorPushBack.y = 0.0f;
					pPlayer->AddPos(vectorPushBack);
				}
				else
				{
					pPlayer->AddPosX(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock);
				}

				// �G�t�F�N�g�F�Ήԁ@�v���C���[���m�̂Ԃ���
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
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// �ΏۃI�u�W�F�N�g���擾
		pPlayerCurrent = Player[cntPlayer];

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
// �n�`�Ƃ̔���
//==============================================================================
void CGame::IsLandField(void)
{
	// ����
	CPlayer*	pPlayerCurrent = nullptr;		// �Ώۃv���C���[
	CBullet*	pBulletCurrent = nullptr;		// �ΏۃI�u�W�F�N�g
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// �Ώۃv���C���[�̎擾
		pPlayerCurrent = Player[cntBullet];

		// �e�����݂��Ȃ���Δ��肵�Ȃ�
		if (NeedsSkipBullet(pPlayerCurrent))
		{
			continue;
		}

		// �ΏۃI�u�W�F�N�g���擾
		pBulletCurrent = pPlayerCurrent->Bullet();

		// ����
		CBullet	bulletHit = *pBulletCurrent;
		PushBackObjectByField(&bulletHit);
		if (bulletHit.Pos().y >= pBulletCurrent->Pos().y)
		{
			// �e�̏��ŏ���
			pPlayerCurrent->ReleaseBullet();

			// �G�t�F�N�g�F�����@�e�ƒn�`�̔���
		}
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
void CGame::PushBackBattleArea(void)
{
	// ����
	CPlayer*	pPlayerCurrent = nullptr;		// �ΏۃI�u�W�F�N�g
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// �ΏۃI�u�W�F�N�g���擾
		pPlayerCurrent = Player[cntPlayer];

		// �Ώۂ̃X�e�[�g���m�F
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// �����߂�
		VECTOR3	vectorPlayerToCenter = Ground->Pos() - pPlayerCurrent->Pos();
		vectorPlayerToCenter.y = 0.0f;
		float	distanceFromCenter = vectorPlayerToCenter.x * vectorPlayerToCenter.x + vectorPlayerToCenter.y * vectorPlayerToCenter.y + vectorPlayerToCenter.z * vectorPlayerToCenter.z;
		if (distanceFromCenter > RADIUS_AREA_BATTLE * RADIUS_AREA_BATTLE)
		{
			float	distancePushBack = sqrtf(distanceFromCenter) - RADIUS_AREA_BATTLE;
			vectorPlayerToCenter.Normalize();
			pPlayerCurrent->AddPos(vectorPlayerToCenter * distancePushBack);
		}
	}
}

//==============================================================================
// �v���C���[����X�L�b�v
//==============================================================================
bool CGame::NeedsSkipPlayer(CPlayer* pPlayer)
{
	// �G���[�`�F�b�N
	if (pPlayer == nullptr)
	{
		return true;
	}

	// �X�e�[�g���m�F
	int		statePlayer = pPlayer->State();
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
	if (!pPlayer->BulletUseFlag())
	{
		return true;
	}

	// �X�L�b�v���Ȃ�
	return false;
}

//------------------------------------------------------------------------------
// EOF
