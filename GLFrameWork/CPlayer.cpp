//------------------------------------------------------------------------------
//
// �v���C���[ [CPlayer.cpp]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include <math.h>

#include "main.h"
#include "CPlayer.h"
#include "Keyboard.h"
#include "Camera.h"
#include "CBullet.h"
#include "CommonGL.h"
#include "Ballistic.h"
#include "ManagerGL.h"
#include "Explosion.h"
const int kHeightMax = 400;
const int kUpSpeed = 3;
const int kDamageCntMax = 60;
const float BARREL_ROTX_SPEED = 1.0f;
const float PLAYER_ROTY_SPEED = 3.0f;
//------------------------------------------------------------------------------
// �R���X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CPlayer::CPlayer() :CModel()
{
	// �ړ����x
	Speed = PLAYER_MOVE_SPEED;

	// �e������
	_Bullet = nullptr;

	// ���U����
	_ReloadTimer = PLAYER_RELOAD_TIME;

	// �C�g��X����]��
	BarrelRotX = 0.0f;


	killCount = 0;
	deathCount = 0;
}

//------------------------------------------------------------------------------
// �f�X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	SafeDelete(Ballistic);
	SafeDelete(_Feed);
}

//------------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	// �g�p�t���O
	PlayerFlag = false;

	// �e�g�p�t���O
	_BulletUseFlag = false;

	LaunchFlag = false;

	// �e
	_Bullet = nullptr;

	// �e��

	Ballistic = CBallistic::Create(COLOR(1.0f,0.0f,0.0f,0.3f));

	// �p�����̏�����
	CModel::Init();

	// �C�g
	Barrel = CModel::Create(TANK_BARREL,_Pos);
	Barrel->Init();
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_YELLOW));


	//����������
	_Hegiht = 0;
	// �̗�
	_PlayerLife = PLAYER_LIFE;
	_Feed = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	_Feed->SetColor(COLOR(0,0,0,0));
	_Timer = 0;
}

//------------------------------------------------------------------------------
// �X�V
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::Update()
{
	//���[�h�I��
	switch (_State)
	{
	case PLAYER_STATE_DEATH:
	{
		AddPosY(kUpSpeed+0.0f);
		_Hegiht += kUpSpeed;
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;

		if (PlayerID == CManager::netData.charNum)
			_Feed->AddAlpha(Alpha);



		Barrel->SetPos(_Pos);			// �ʒu
		CManager::SendPos(_Pos);
		CManager::SendRot(_Rot);
		CManager::SendCannonRot(Barrel->Rot());
		CManager::SendCannon(LaunchFlag);





		if (_Hegiht > kHeightMax)
		{
			SetRespawn();
		}
		return;
	}
	case PLAYER_STATE_RESPAWN:
	{
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;

		AddPosY(-kUpSpeed);
		_Hegiht += kUpSpeed;


		if (PlayerID == CManager::netData.charNum)
			_Feed->AddAlpha(-Alpha);





		Barrel->SetPos(_Pos);			// �ʒu
		CManager::SendPos(_Pos);
		CManager::SendRot(_Rot);
		CManager::SendCannonRot(Barrel->Rot());
		CManager::SendCannon(LaunchFlag);






		if (_Hegiht > kHeightMax)
		{
			_State = PLAYER_STATE_WAIT;
			_Feed->SetAlpha(0);
		}
		return;
	}
	case PLAYER_STATE_DAMAGE:
	{
		_Timer++;
		_State = PLAYER_STATE_WAIT;
		break;
	}
	default:
		break;
	}

	if (_Timer != 0)
	{
		_Timer++;
		if (_Timer % 2 == 0)
		{
			Barrel->SetDrawFlag(true);
			SetDrawFlag(true);
		}
		else{
			Barrel->SetDrawFlag(false);
			SetDrawFlag(false);
		}
		if (_Timer > kDamageCntMax)
		{
			Barrel->SetDrawFlag(true);
			SetDrawFlag(true);
			_Timer = 0;
		}

	}

	if (PlayerFlag == true)
	{
		// ����L�����N�^�[�̍X�V
		UpdatePlayer();
	}
	else
	{
		// ����L�����N�^�[�ł͂Ȃ���CPU
		UpdateCPU();
	}
}

//------------------------------------------------------------------------------
// �L�����N�^�[������L�����������ꍇ�̍X�V
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::UpdatePlayer(void)
{
	// �e�̎g�p��Ԃ��m��
	if (_Bullet == nullptr)
	{
		_BulletUseFlag = false;
	}

	// ������
	VECTOR3 rot = VECTOR3(0.0f,0.0f,0.0f);

	// �ړ�
	// ��
	if (CKeyboard::GetPress(DIK_W))
	{
		Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed;
		Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed;
	}

	// ��
	else if (CKeyboard::GetPress(DIK_S))
	{
		Movement.x -= sinf(DEG2RAD(_Rot.y)) * Speed;
		Movement.z -= cosf(DEG2RAD(_Rot.y)) * Speed;
	}

	// ��
	if (CKeyboard::GetPress(DIK_A))
	{
		rot.y += PLAYER_ROTY_SPEED;
	}

	// �E
	else if (CKeyboard::GetPress(DIK_D))
	{
		rot.y -= PLAYER_ROTY_SPEED;
	}

	// �C�g�̏㉺
	if (CKeyboard::GetPress(DIK_UP))
	{
		BarrelRotX -= BARREL_ROTX_SPEED;
	}
	else if (CKeyboard::GetPress(DIK_DOWN))
	{
		BarrelRotX += BARREL_ROTX_SPEED;
	}

	// �L�����N�^�[�̉�]
	AddRot(rot);

	// �l�̊ۂߍ���
	// �v���C���[�̉�]��
	if (Rot().y > 360.0f)
	{
		SetRotY(Rot().y - 2 * 360.0f);
	}
	else if (Rot().y < -360.0f)
	{
		SetRotY(Rot().y + 2 * 360.0f);
	}

	// �C�g
	if (BarrelRotX > BARREL_ROT_MIN)
	{
		BarrelRotX = BARREL_ROT_MIN;
	}
	else if (BarrelRotX < BARREL_ROT_MAX)
	{
		BarrelRotX = BARREL_ROT_MAX;
	}

	// �L�����N�^�[�̈ړ��l�����Z
	AddPos(Movement);

	// ����
	Movement *= 0.95f;

	// �C�g�̈ʒu�A��]���X�V
	Barrel->SetPos(_Pos);			// �ʒu
	Barrel->SetRot(_Rot);			// ��]
	Barrel->AddRotX(BarrelRotX);	// ��Őݒ肵����]�ʂɖC�g��X����]�ʂ����Z

	// �e���̍X�V
	Ballistic->Update(_Pos,VECTOR3(BarrelRotX,_Rot.y,_Rot.z));

	// �e�̔���
	// �e�����˂���Ă��Ȃ�������
	if (LaunchFlag == false)
	{
		if (CKeyboard::GetTrigger(DIK_SPACE))
		{
			_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
			LaunchFlag = true;
			_BulletUseFlag = true;
			_ReloadTimer = 0;
			CManager::SendCannon(LaunchFlag);
		}
	}
	// �e�����˂���Ă��鎞
	else
	{
		// �����[�h�\�܂ł̎��Ԃ����Z
		_ReloadTimer++;

		// �����[�h�������Ԃ𒴂�����
		if (_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			// �Ĕ��ˉ\��
			LaunchFlag = false;
			_BulletUseFlag = false;
			_ReloadTimer = PLAYER_RELOAD_TIME;
		}
	}

	CManager::SendPos(_Pos);
	CManager::SendRot(_Rot);
	CManager::SendCannonRot(Barrel->Rot());

#ifdef _DEBUG
	// �f�o�b�O�p
	// �A��
	if (CKeyboard::GetPress(DIK_V))
	{
		CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
	}

	// ���C�t�̌��Z
	if (CKeyboard::GetPress(DIK_L))
	{
		this->AddPlayerLife(-1);
		_State = PLAYER_STATE_DAMAGE;
	}

	// �e�̍폜�m�F
	if (CKeyboard::GetPress(DIK_M))
	{
		this->ReleaseBullet();
	}

#endif
}

//=============================================================================
//	�e���ˏ���
//=============================================================================
void CPlayer::BlastBullet()
{
	if (LaunchFlag == false)
	{
		_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
		LaunchFlag = true;
		_BulletUseFlag = true;
		_ReloadTimer = 0;
	}
}

//------------------------------------------------------------------------------
// �X�V
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::UpdateCPU(void)
{
	Barrel->SetPos(_Pos);			// �ʒu
	BarrelRotX = Barrel->Rot().x;
	// �e�����˂���Ă��Ȃ�������
	if (LaunchFlag == true)
	{
		// �����[�h�\�܂ł̎��Ԃ����Z
		_ReloadTimer++;

		// �����[�h�������Ԃ𒴂�����
		if (_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			// �Ĕ��ˉ\��
			LaunchFlag = false;
			_BulletUseFlag = false;
			_ReloadTimer = PLAYER_RELOAD_TIME;
		}
	}
}

void CPlayer::setBarrelRot(VECTOR3 _rot)
{
	Barrel->SetRot(_rot);			// ��]
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::ReleaseBullet(void)
{
	// �e���g�p���Ă�����폜
	if (_Bullet != nullptr)
	{
		SafeRelease(_Bullet);
		_BulletUseFlag = false;
	}
}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
// ����
//  modelID		: �������������f����ID
//  pos			: �����ʒu
//  playerID	: �v���C���[��ID�i�ʂ��ԍ��j
// �߂�l
//  CPlayer	: ���������v���C���[�̃|�C���^
//------------------------------------------------------------------------------
CPlayer* CPlayer::Create(int modelID,const VECTOR3& pos,int playerID)
{
	CPlayer* model = new CPlayer;

	if (model == nullptr)
	{
		return nullptr;
	}

	model->ModelID = modelID;
	model->PlayerID = playerID;
	model->_Pos = pos;
	model->Init();

	return model;
}
//------------------------------------------------------------------------------
// ���S����
//------------------------------------------------------------------------------
// ����
// �Ȃ�
// �߂�l
// �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::SetDeath(VECTOR3 pos, int _charNum)
{
	if (_State != PLAYER_STATE_DEATH)
	{
		_Hegiht = 0;
		_State = PLAYER_STATE_DEATH;
		_PlayerRespown = pos;


		if (_charNum == CManager::netData.charNum)
			_Feed->SetAlpha(0);
	}
}
//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
// ����
// pos			: �����ʒu
// �߂�l
// �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::SetRespawn(void)
{
	_Hegiht = 0;
	_State = PLAYER_STATE_RESPAWN;
	_PlayerRespown.y += kHeightMax;
	SetPos(_PlayerRespown);
	_PlayerLife = PLAYER_LIFE;

	if (PlayerID == CManager::netData.charNum)
		_Feed->SetAlpha(1);
	
	Movement = VECTOR3(0,0,0);



	Barrel->SetPos(_Pos);			// �ʒu
	CManager::SendPos(_Pos);
	CManager::SendRot(_Rot);
	CManager::SendCannonRot(Barrel->Rot());
	CManager::SendCannon(LaunchFlag);

}
//------------------------------------------------------------------------------
// ���Z����
//------------------------------------------------------------------------------
// ����
// addVal			: ���C�t
// �߂�l
// �Ȃ�
//------------------------------------------------------------------------------

void CPlayer::AddPlayerLife(int addVal)
{
	if (_Timer != 0 || _State != PLAYER_STATE_DAMAGE)
	{
		_PlayerLife += addVal;
		if (_PlayerLife < 0)
		{
			_PlayerLife = 0;
		}
		else
			if (_PlayerLife > PLAYER_LIFE)
			{
				_PlayerLife = PLAYER_LIFE;
			}
	}
}

//------------------------------------------------------------------------------
// EOF