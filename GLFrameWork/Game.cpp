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
#include "Explosion.h"
#include "Spark.h"
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
#include "SoundAL.h"

#include "UI.h"

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 2500.0f;   // ��̔��a
CPlayer** CGame::Player;
CPlayer* Player = nullptr;//�v���C���[
const float	CGame::RADIUS_DEFENSE_CHARACTER = 12.0f;	// �L�����N�^�[�̖h�䔼�a
const float	CGame::HEIGHT_DEFENSE_CHARACTER = 0.0f;		// �L�����N�^�[�̖h�䒆�S����
const float	CGame::RADIUS_OFFENSE_BULLET = 10.0f;		// �C�e�̍U�����a
const float	CGame::HEIGHT_OFFENSE_BULLET = 0.0f;		// �C�e�̍U�����S����
const float	CGame::RADIUS_PUSH_CHARACTER = 10.0f;		// �L�����N�^�[�̉����߂����a
const float	CGame::HEIGHT_PUSH_CHARACTER = 0.0f;		// �L�����N�^�[�̉����߂����S����
const float	CGame::RADIUS_DEFENSE_ROCK = 28.0f;			// ��̖h�䔼�a
const float	CGame::HEIGHT_DEFENSE_ROCK = 0.0f;			// ��̖h�䒆�S����
const float	CGame::RADIUS_PUSH_ROCK = 38.0f;			// ��̉����߂����a
const float	CGame::HEIGHT_PUSH_ROCK = 0.0f;				// ��̉����߂����S����
const float CGame::FIELD_PANEL_SIZE = 35.0f;			//�t�B�[���h�̃p�l���ꖇ�̃T�C�Y
const float	CGame::HEIGHT_PLAYER_TO_FIELD = 10.0f;		// �v���C���[�ƒn�ʂ̍�

const float	CGame::RADIUS_AREA_BATTLE = 1000.0f;		// �퓬�G���A���a
const float	CGame::HEIGHT_WALL = 500.0f;				// �ǂ̍���

const int	CGame::MAX_ROCK = 20;						// ��̐�

const VECTOR3 CGame::ROCK_POSITION_LIST[] = {
	VECTOR3(-214.0f,100.0f,421.0f),
	VECTOR3(359.0f,100.0f,188.0f),
	VECTOR3(94.0f,100.0f,-458.0f),
	VECTOR3(-198.0f,100.0f,222.0f),
	VECTOR3(419.0f,100.0f,293.0f),
	VECTOR3(-335.0f,100.0f,164.0f),
	VECTOR3(-471.0f,100.0f,-115.0f),
	VECTOR3(368.0f,100.0f,-363.0f),
	VECTOR3(-476.0f,100.0f,231.0f),
	VECTOR3(-249.0f,100.0f,-319.0f),
	VECTOR3(-243.0f,100.0f,481.0f),
	VECTOR3(345.0f,100.0f,-253.0f),
	VECTOR3(444.0f,100.0f,-118.0f),
	VECTOR3(186.0f,100.0f,27.0f),
	VECTOR3(387.0f,100.0f,-438.0f),
	VECTOR3(-12.0f,100.0f,-439.0f),
	VECTOR3(496.0f,100.0f,-332.0f),
	VECTOR3(-247.0f,100.0f,143.0f),
	VECTOR3(-302.0f,100.0f,-296.0f),
	VECTOR3(-171.0f,100.0f,-274.0f),

};

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
	CSoundAL::Play(CSoundAL::BGM_GAME);
	//�n�`����
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(FIELD_PANEL_SIZE,FIELD_PANEL_SIZE),VECTOR2(0,0),1.5f);
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// �󐶐�
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	// ���E������
	CylinderArea = nullptr;
	CylinderArea = CBattleAreaCylinder::Create(VECTOR3(0.0f,0.0f,0.0f),HEIGHT_WALL,VECTOR2(64.0f,1.0f),RADIUS_AREA_BATTLE,VECTOR2(1,-0.5f));
	CylinderArea->SetTex(CTexture::Texture(TEX_WALL));
	CylinderArea->SetAlpha(0.5f);

#ifdef _DEBUG
	// �f�o�b�O���C���[�t���[��
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	Player = new CPlayer*[PLAYER_MAX];
	// �v���C���[����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (i % 2 == 1)
			Player[i] = CPlayer::Create(CModel::RINCHAN,VECTOR3(0.0f + i * 50.0f,30.0f,0.0f),i);
		else
			Player[i] = CPlayer::Create(CModel::YOUJO, VECTOR3(0.0f + i * 50.0f, 30.0f, 0.0f), i);

		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_RED + i));
		Player[i]->SetRot(0.0f,180.0f,0.0f);
		Player[i]->setBarrelTex(TEX_YOUJO_RED + i);

		if (i == CManager::netData.charNum)
		{
			Player[i]->SetPlayerFlag(true);
		}
	}	//�v���C���[�J��������
	CPlayerCamera::Create(Player[CManager::netData.charNum],35.0f);

	// UI������
	UI = new CUI;
	UI->Init();
	UI->setMyID(CManager::netData.charNum);
	UI->SetPlayer(Player);
	UI->MiniMap()->SetFieldSize(VECTOR3(RADIUS_AREA_BATTLE*2.25f,0,RADIUS_AREA_BATTLE*2.25f));

	// ��̐���
	ppRock_ = new CModel*[MAX_ROCK];

	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		ppRock_[cntRock] = CModel::Create(CModel::ROCK,ROCK_POSITION_LIST[cntRock]);
		ppRock_[cntRock]->SetScl(1,1,1);
		ppRock_[cntRock]->SetTex(CTexture::Texture(TEX_ROCK));
		PushBackObjectByField(ppRock_[cntRock]);
	}

	CManager::gameStartFlag = true;

	CManager::sendGameStart();
}

//=============================================================================
//	�v���C���[���̃Z�b�g�֐�
//=============================================================================
void CGame::SetPlayerState(NET_DATA _netData,DATA_TYPE _dataType)
{
	//	�����ȊO�̃f�[�^�Ȃ�
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

		case DATA_TYPE_CANNONROT:

			Player[_netData.charNum]->setBarrelRot(
				VECTOR3(_netData.data_cannonRot.rotX,
				_netData.data_cannonRot.rotY,
				_netData.data_cannonRot.rotZ
				));

			break;

		case DATA_TYPE_CANNON:

			if (_netData.data_cannon.flag == true)
			{
				Player[_netData.charNum]->BlastBullet();
			}

			break;

		case DATA_TYPE_DEATH:

			break;

		case DATA_TYPE_KILL:

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
	
	CManager::SendKillDeath(Player[CManager::netData.charNum]->getKillCount(),
							Player[CManager::netData.charNum]->getDeathCount());

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
	for (int i = 0; i < PLAYER_MAX; i++)
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
	const float rate = currentTimer / maxTimer;


	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		if (CManager::netData.charNum == 0)
		{
			CManager::SendChangeResult();
			CManager::ChangeScene(SCENE_RESULT);
		}
	}

	// ��̈ʒu�v���C���[�ɍ��킹��
	Sky->SetPosX(Player[0]->Pos().x);
	Sky->SetPosZ(Player[0]->Pos().z);
	for (int loop = 0;loop < PLAYER_MAX;loop++)
	{
		if (Player[loop]->PlayerLife() <= 0)
		{
			VECTOR3	NormalGround;		// �n�`�̖@��
			VECTOR3 Respawn = VECTOR3(rand() % 100 + 0.0f,0,rand() % 100 + 0.0f);
			Respawn.y = Ground->GetHeight(Respawn,&NormalGround);

			Player[loop]->SetDeath(Respawn, loop);
		}
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
				pPlayerDefense->AddPlayerLife(-1);
				pPlayerDefense->SetState(PLAYER_STATE_DAMAGE);

				// �G�t�F�N�g�F�����@�e���v���C���[�ɓ��������Ƃ�
				CExplosion::Create(pPlayerDefense->Pos());

				//	����
				if (pPlayerDefense->PlayerLife() == 0)
				{
					//	�E���ꂽ�����Z
					pPlayerDefense->addDeathCount();
					CManager::SendDeath(pPlayerDefense->getDeathCount(), pPlayerDefense->getPlayerID());

					//	�E���������Z
					pPlayerOffense->addKillCount();
					CManager::SendKill(pPlayerOffense->getKillCount(), pPlayerOffense->getPlayerID());
				}

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

				// �G�t�F�N�g�F�����@�e����ɓ��������Ƃ�
				CExplosion::Create(positionOffense);

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
			CPlayer*	pPlayerDefense = Player[cntPlayer];		// �h�䑤�v���C���[

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
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);
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
			CObject*	pRock = ppRock_[cntRock];		// ��I�u�W�F�N�g

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
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);

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

				// �G�t�F�N�g�F�Ήԁ@�v���C���[�Ɗ�̂Ԃ���
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
			CExplosion::Create(pBulletCurrent->Pos());
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
	HeightGround = Ground->GetHeight(pObject->Pos(), &NormalGround) + HEIGHT_PLAYER_TO_FIELD;

	//********************************************************
	// 2015_02_12 �p������p�̏�����ǉ� ��������
	//********************************************************
	// ��]�����߂�
	VECTOR3	vectorUp(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	VECTOR3	vectorAxisRotation;				// ��]��
	float	rotation = 0.0f;				// ��]��
	VECTOR3::Cross(&vectorAxisRotation, NormalGround, vectorUp);
	if (vectorAxisRotation.x < FLT_EPSILON && vectorAxisRotation.x > -FLT_EPSILON)
	{
		if (vectorAxisRotation.z < FLT_EPSILON && vectorAxisRotation.z > -FLT_EPSILON)
		{
			if (vectorAxisRotation.y < FLT_EPSILON && vectorAxisRotation.y > -FLT_EPSILON)
			{
				vectorAxisRotation.y = 1.0f;
			}
		}
	}
	vectorAxisRotation.Normalize();
	rotation = VECTOR3::Dot(NormalGround, vectorUp);
	if (rotation > 2.0f * FLT_EPSILON || rotation < -2.0f * FLT_EPSILON)
	{
		rotation = RAD_TO_DEG * acosf(rotation);
	}
	else
	{
		rotation = 0.0f;
	}

	// �L�����N�^�[�ɐݒ肷��
	pObject->SetPosY(HeightGround);
	pObject->SetAxisRotation(vectorAxisRotation);
	pObject->SetRotationAxis(rotation);
	//********************************************************
	// 2015_02_12 �p������p�̏�����ǉ� �����܂�
	//********************************************************
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
