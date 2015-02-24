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
#include "Input/VC.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Explosion.h"
#include "Smoke.h"
#include "Ballistic.h"
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
#include "Shadow.h"

#include "UI.h"

// �ÓI�����o�ϐ�
const float CGame::RADIUS_SKY = 2500.0f;   // ��̔��a
CUI* CGame::UI = nullptr;
CPlayer** CGame::Player;
CPlayer* Player = nullptr;//�v���C���[

CGame::PHSE CGame::gamePhase;

int CGame::gamePhaseCnt = 0;
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

const VECTOR3 CGame::PLAYER_POSITION_LIST[PLAYER_MAX] = {
 VECTOR3(-0.0f,100.0f,800),
 VECTOR3(690,100.0f,400),
 VECTOR3(690,100.0f,-400),
 VECTOR3(0,100.0f,-800),
 VECTOR3(-700,100.0f,-400),
 VECTOR3(-700,100.0f,400),
};

const VECTOR3 CGame::PLAYER_ROTATION_LIST[PLAYER_MAX] = {
 VECTOR3(0,180,0),
 VECTOR3(0,240.0f,0),
 VECTOR3(0,300.0f,0),
 VECTOR3(0,0,0),
 VECTOR3(0,60.0f,0),
 VECTOR3(0,120.0f,0),
};

const VECTOR3 CGame::ROCK_POSITION_LIST[] = {
	VECTOR3(-214.0f,100.0f,421.0f),
	VECTOR3(359.0f,100.0f,188.0f),
	VECTOR3(94.0f,100.0f,-458.0f),
	VECTOR3(-198.0f,100.0f,222.0f),
	VECTOR3(419.0f,100.0f,293.0f),
	VECTOR3(-325.0f,100.0f,164.0f),
	VECTOR3(-471.0f,100.0f,-115.0f),
	VECTOR3(368.0f,100.0f,-373.0f),
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

const VECTOR3 CGame::ROCK_ROTATION_LIST[] = {
	VECTOR3(10.0f, 10.0f, 10.0f),
	VECTOR3(20.0f, 20.0f, 20.0f),
	VECTOR3(30.0f, 30.0f, 30.0f),
	VECTOR3(40.0f, 40.0f, 40.0f),
	VECTOR3(50.0f, 50.0f, 50.0f),
	VECTOR3(60.0f, 95.0f, 60.0f),
	VECTOR3(70.0f, 70.0f, 70.0f),
	VECTOR3(80.0f, 110.0f, 80.0f),
	VECTOR3(90.0f, 90.0f, 90.0f),
	VECTOR3(100.0f, 100.0f, 100.0f),
	VECTOR3(110.0f, 110.0f, 110.0f),
	VECTOR3(120.0f, 120.0f, 120.0f),
	VECTOR3(130.0f, 130.0f, 130.0f),
	VECTOR3(140.0f, 140.0f, 140.0f),
	VECTOR3(150.0f, 150.0f, 150.0f),
	VECTOR3(160.0f, 160.0f, 160.0f),
	VECTOR3(170.0f, 170.0f, 170.0f),
	VECTOR3(180.0f, 185.0f, 180.0f),
	VECTOR3(190.0f, 190.0f, 190.0f),
	VECTOR3(200.0f, 200.0f, 200.0f)
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
	gamePhase = PHASE_3;
	gamePhaseCnt = 0;
	gameEndCount = 0;
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

	Player = new CPlayer*[PLAYER_MAX];
	// �v���C���[����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (i % 2 == 1)
			Player[i] = CPlayer::Create(CModel::RINCHAN,PLAYER_POSITION_LIST[i],i);
		else
			Player[i] = CPlayer::Create(CModel::YOUJO,PLAYER_POSITION_LIST[i],i);

		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_RED + i));
		Player[i]->SetRot(PLAYER_ROTATION_LIST[i]);
		Player[i]->setBarrelTex(TEX_YOUJO_RED + i);

		if (i == CManager::netData.charNum)
		{
			Player[i]->SetPlayerFlag(true);
			Player[i]->CreateBallistic();
		}
	}
		//�v���C���[�J��������
	CPlayerCamera::Create(Player[CManager::netData.charNum],35.0f);

	// �y�e�X�g�z�e�v���C���[�̐F���Z�b�g
	for(int i = 0; i < PLAYER_MAX; i++)
	{
		switch(i)
		{
			// ��
			case 0:
				Player[i]->SetPlayerColor(RED(0.5f));
				break;

			// ��
			case 1:
				Player[i]->SetPlayerColor(BLUE(0.5f));
				break;

			// ��
			case 2:
				Player[i]->SetPlayerColor(CYAN(0.5f));
				break;

			// ��
			case 3:
				Player[i]->SetPlayerColor(COLOR(1.0f, 0.7f, 0.0f, 0.5f));
				break;
		
			// ��
			case 4:
				Player[i]->SetPlayerColor(WHITE(0.5f));
				break;

			// P
			case 5:
				Player[i]->SetPlayerColor(YELLOW(0.5f));
				break;

			default:
				break;
		}
	}

	// �v���C���[�̓��͂��~�߂�
	for (int i = 0; i < PLAYER_MAX; i++){
		Player[i]->SetInputFlag(false);
	}

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
		ppRock_[cntRock]->SetRot(ROCK_ROTATION_LIST[cntRock]);
		ppRock_[cntRock]->SetScl(1,1,1);
		ppRock_[cntRock]->SetTex(CTexture::Texture(TEX_ROCK));
		PushBackObjectByField(ppRock_[cntRock], 10.0f);
	}

	// �e����
	Shadow = new CShadow*[2 * PLAYER_MAX];
	for (int cntShadow = 0; cntShadow < 2 * PLAYER_MAX; ++cntShadow)
	{
		Shadow[cntShadow] = nullptr;
		Shadow[cntShadow] = CShadow::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(1.0f, 1.0f));
		if (cntShadow < PLAYER_MAX)
		{
			Shadow[cntShadow]->SetScl(30, 30, 30);
		}
		else
		{
			Shadow[cntShadow]->SetScl(20, 20, 20);
		}
		Shadow[cntShadow]->SetTex(CTexture::Texture(TEX_SHADOW));
	}

	CManager::gameStartFlag = true;

	CManager::sendGameStart();
}

void CGame::subTimer()
{
	//	UI�̃^�C�}�[���炷����
	UI->SubTime();
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

			/*Player[_netData.charNum]->SetAxisRotation(VECTOR3(_netData.data_rot.X, _netData.data_rot.Y, _netData.data_rot.Z));
			Player[_netData.charNum]->SetRotationAxis(_netData.data_rot.rot);
			Player[_netData.charNum]->SetRotY(_netData.data_rot.yRotation);*/
			Player[_netData.charNum]->SetRotY(_netData.data_rot.rotY);
			
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

	// �e�̔j��
	for (int chtShadow = 0; chtShadow < 2 * PLAYER_MAX; ++chtShadow)
	{
		SafeDelete(Shadow[chtShadow]);
	}
	SafeDelete(Shadow);

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

	SafeRelease(Sky);

	// �n�`�j��
	SafeRelease(Ground);

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
	VC* vc = VC::Instance();
	// �ŏ��̃J�E���g�_�E��
	StartCount();

	if (vc->Trigger(COMMAND_OK))
	{
		if (CManager::netData.charNum == 0)
		{
			CManager::SendChangeResult();
			CManager::ChangeScene(SCENE_RESULT);
		}
	}

	// ��̈ʒu�v���C���[�ɍ��킹��
	Sky->SetPosX(Player[CManager::netData.charNum]->Pos().x);
	Sky->SetPosZ(Player[CManager::netData.charNum]->Pos().z);
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

	// ���e�n�_����
	HitBulletToField();

	// �e�����킹��
	for (int cntShadow = 0; cntShadow < PLAYER_MAX; ++cntShadow)
	{
		VECTOR3	positionShadow = Player[cntShadow]->Pos();
		positionShadow.y -= HEIGHT_PLAYER_TO_FIELD;
		Shadow[cntShadow]->SetPos(positionShadow);
		PushBackObjectByField(Shadow[cntShadow], 1.0f);
	}
	for (int cntShadow = 0; cntShadow < PLAYER_MAX; ++cntShadow)
	{
		if (NeedsSkipBullet(Player[cntShadow]))
		{
			Shadow[cntShadow + PLAYER_MAX]->SetPos(VECTOR3(0.0f, 1000.0f, 0.0f));
			continue;
		}
		VECTOR3	positionShadow = Player[cntShadow]->Bullet()->Pos();
		Shadow[cntShadow + PLAYER_MAX]->SetPos(positionShadow);
		PushBackObjectByField(Shadow[cntShadow + PLAYER_MAX], 1.0f);
	}

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
				if (pPlayerDefense->PlayerLife() > 0)
				{
					if (pPlayerDefense->GetPreyerFlag())
					{
						VC::Instance()->SetVibration(0.7f,30,0.7f,30);
					}
					CSoundAL::Play(CSoundAL::SE_HIT,pPlayerDefense->Pos());
					CSoundAL::Play(CSoundAL::SE_DAMAGE,pPlayerDefense->Pos());
				}
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

					if (CManager::userInfo[pPlayerDefense->getPlayerID()].entryFlag == false)
					{
						if (CManager::netData.charNum == 0)
							CManager::SendDeathFlag(pPlayerDefense->getPlayerID());
					}
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
				CSmoke::Create(positionOffense);

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
		PushBackObjectByField(pPlayerCurrent, HEIGHT_PLAYER_TO_FIELD);
	}
}

//==============================================================================
// �n�`�Ƃ̔���
//==============================================================================
void CGame::IsLandField(void)
{
	// ����
	VECTOR3	 nor;			// �@��
	float	 height;		// ����
	VECTOR3	 bulletPos;		// �ΏۃI�u�W�F�N�g���W

	CPlayer* pPlayerCurrent = nullptr;		// �Ώۃv���C���[

	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// �Ώۃv���C���[�̎擾
		pPlayerCurrent = Player[cntBullet];

		// �e�����݂��Ȃ���Δ��肵�Ȃ�
		if (NeedsSkipBullet(pPlayerCurrent))
		{
			continue;
		}

		// �ΏۃI�u�W�F�N�g���W���擾
		bulletPos = pPlayerCurrent->Bullet()->Pos();

		// ����
		height = Ground->GetHeight(bulletPos - VECTOR3(0.0f, BULLET_SIZE * 0.5f, 0.0f), &nor);
		if (height >= bulletPos.y)
		{
			// �e�̏��ŏ���
			CSoundAL::Play(CSoundAL::SE_IMPACT, bulletPos);
			pPlayerCurrent->ReleaseBullet();

			// �G�t�F�N�g�F�����@�e�ƒn�`�̔���
			CSmoke::Create(VECTOR3(bulletPos.x, height, bulletPos.z));
		}
	}
}

//==============================================================================
// �I�u�W�F�N�g�̒n�`�ɂ�鉟���߂�
//==============================================================================
void CGame::PushBackObjectByField(CObject* pObject, float offsetY)
{
	// �n�`�Ƃ̂����蔻��
	VECTOR3	NormalGround;		// �n�`�̖@��
	float	HeightGround;		// �n�`�̍���
	HeightGround = Ground->GetHeight(pObject->Pos(), &NormalGround) + offsetY;

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
	if (rotation <= 1.0f && rotation >= -1.0f)
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
		if (distanceFromCenter > (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER) * (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER))
		{
			float	distancePushBack = sqrtf(distanceFromCenter) - (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER);
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

//==============================================================================
// ���e�n�_����
//==============================================================================
void CGame::HitBulletToField(void)
{
	VECTOR3		nor;
	float		height;
	CBillboard* mark;
	VECTOR3		markPos;
	CBallistic* ballistic = Player[CManager::netData.charNum]->GetBallistic();
	CPolygon3D* landing = ballistic->GetLanding();

	for(int cnt = 0; cnt < MARK_MAX; ++cnt)
	{
		// ������
		nor = VECTOR3(0.0f,0.0f,0.0f);
		height = 0.0f;

		// �}�[�N���
		mark = ballistic->GetMark(cnt);
		markPos = mark->Pos();

		// ��������
		height = Ground->GetHeight(markPos - VECTOR3(0.0f, BULLET_SIZE * 0.5f, 0.0f), &nor);
		if(height >= markPos.y)
		{
			// ��]�����߂�
			VECTOR3	vectorUp(0.0f, 1.0f, 0.0f);		// ������x�N�g��
			VECTOR3	vectorAxisRotation;				// ��]��
			float	rotation = 0.0f;				// ��]��
			VECTOR3::Cross(&vectorAxisRotation, nor, vectorUp);
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
			rotation = VECTOR3::Dot(nor, vectorUp);
			if (rotation <= 1.0f && rotation >= -1.0f)
			{
				rotation = RAD_TO_DEG * acosf(rotation);
			}
			else
			{
				rotation = 0.0f;
			}

			// ���e�}�[�N�ɐݒ肷��
			landing->SetPos(VECTOR3(markPos.x, height + 0.5f, markPos.z));
			landing->SetAxisRotation(vectorAxisRotation);
			landing->SetRotationAxis(rotation);
			break;
		}
	}
}
//==============================================================================
// �ŏ��̃J�E���g�_�E���̂��
//==============================================================================
void CGame::StartCount(void)
{
	const int PHASE_COUNT_3 = 60 * 1;
	const int PHASE_COUNT_2 = 60 * 2;
	const int PHASE_COUNT_1 = 60 * 3;
	const int PHASE_COUNT_START = 60 * 4;
	const int PHASE_COUNT_START_FIN = 60 * 5;
	const int PHASE_COUNT_END = 60 * 3;

	switch (gamePhase){

		case PHASE_NONE:
		{
			break;
		}
		case PHASE_3:
		{
			if (gamePhaseCnt == PHASE_COUNT_3){
				UI->SetNumber(3);
				UI->SetNumberDrawFlag(true);
				UI->SetTimeUpdateFlag(false);
				UI->SetCountSizeRefresh();
				gamePhase = PHASE_2;
				CSoundAL::Play(CSoundAL::SE_COUNT_DOWN);
			}
			break;
		}
		case PHASE_2:
		{
			if (gamePhaseCnt == PHASE_COUNT_2){
				UI->SetNumber(2);
				UI->SetCountSizeRefresh();
				gamePhase = PHASE_1;
				CSoundAL::Play(CSoundAL::SE_COUNT_DOWN);
			}
			break;
		}
		case PHASE_1:
		{
			if (gamePhaseCnt == PHASE_COUNT_1){
				UI->SetNumber(1);
				UI->SetCountSizeRefresh();
				gamePhase = PHASE_START;
				CSoundAL::Play(CSoundAL::SE_COUNT_DOWN);
			}
			break;
		}
		case PHASE_START:
		{
			if (gamePhaseCnt == PHASE_COUNT_START){
				UI->SetStringDrawFlag(true);
				UI->SetNumber(0);
				UI->SetNumberDrawFlag(false);
				UI->SetTimeUpdateFlag(true);
				gamePhase = PHASE_START_FIN;
				CSoundAL::Play(CSoundAL::SE_START);

				CManager::SendReborn(1);
				CManager::SendReborn(2);
				CManager::SendReborn(3);
				CManager::SendReborn(4);
				CManager::SendReborn(5);
				// �v���C���[�̓��͂��~�߂�
				for (int i = 0; i < PLAYER_MAX; i++){
					Player[i]->SetInputFlag(true);
				}
			}
				break;
		}
		case PHASE_START_FIN:
		{
			if (gamePhaseCnt == PHASE_COUNT_START_FIN){
				UI->SetStringDrawFlag(false);
				gamePhase = PHASE_PLAY;
			}
			break;
		}
		case PHASE_PLAY:
		{
			if (UI->GetTime() == 0){
				gamePhase = PHASE_END;
				UI->SetStringDrawFlag(true);
				UI->SetStringTexture(CTexture::Texture(TEX_END));

				UI->SetTimeUpdateFlag(false);
				UI->ReportDrawDisable();

				// �v���C���[�̍U�����~�߂�
				for (int i = 0; i < PLAYER_MAX; i++){
					Player[i]->SetEndGameFlag(true);
				}
			}
			break;
		}
		case PHASE_END:
		{
			gameEndCount++;
			if (gameEndCount >= PHASE_COUNT_END){
				CManager::ChangeScene(SCENE_RESULT);
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
// EOF
