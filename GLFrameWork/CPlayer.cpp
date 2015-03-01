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
#include<float.h>

#include "main.h"
#include "CPlayer.h"
#include "Camera.h"
#include "CBullet.h"
#include "CommonGL.h"
#include "ManagerGL.h"
#include "Explosion.h"
#include "SandCloud.h"
#include "SoundAL.h"
#include "Input\VC.h"
#include "Input\Keyboard.h"
#include "Object.h"

#define NARI_SCL (30.0f)
const int kHeightMax = 400;
const int kUpSpeed = 3;
const int kDamageCntMax = 60;
const float BARREL_ROTX_SPEED = 1.0f;
const float PLAYER_ROTY_SPEED = 1.0f;
const int NARI_SCL_SPEED = 6;
//------------------------------------------------------------------------------
//�萔��`
//------------------------------------------------------------------------------
const float TURN_SPEED = 1.0f;		//��]���x
const int UserMax = 6;//�ő僆�[�U�[��
const double SURCH_MAX = 300.0f;	//�t�B�[���h�̃T�C�Y��1/4
const float GRAVITY = 0.06f;				//�e�ɂ�����d�͉����x
const float FIELD_PANEL_SIZE = 35.0f;			//�t�B�[���h�̃p�l���ꖇ�̃T�C�Y
const float MAXANGLE = ( -45.0f );
const float MAX_RANGE = 202.0f;
const float MIN_RANGE = 70.0f;

int CPlayer::MaxId = 0;
CPlayer* CPlayer::player[PLAYER_MAX];

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

	DriveSE = nullptr;
	IdlingSE = nullptr;

	// ���U����
	_ReloadTimer = PLAYER_RELOAD_TIME;

	// �C�g��X����]��
 BarrelDestRotX = BarrelRotX = 0.0f;


	killCount = 0;
	deathCount = 0;
	_SandTime = 0;
}

//------------------------------------------------------------------------------
// �f�X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
 player[MyId] = nullptr;
 MaxId--;
 if(MaxId < 0)
 {
  MaxId = 0;
 }
	SafeRelease(DriveSE);
	SafeRelease(IdlingSE);
	SafeDelete(Ballistic);
	SafeDelete(_Feed);
	SafeRelease(_nari);
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

	// �e���˃t���O
	_LaunchFlag = false;

	// �Q�[���I���t���O
	_EndGameFlag = false;

	// �e
	_Bullet = nullptr;

	// �e��
	Ballistic = nullptr;

	// �p�����̏�����
	CModel::Init();

	// �C�g
	Barrel = CModel::Create(TANK_BARREL,_Pos);
	Barrel->Init();
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_YELLOW));

	_nari = CBillboard::Create(_Pos,VECTOR2(512 / NARI_SCL,1024 / NARI_SCL));
	_nari->SetTex(CTexture::Texture(TEX_NARITADA));
	_nari->SetAlpha(0.0f);

	//����������
	_Hegiht = 0;

	// �̗�
	_PlayerLife = PLAYER_LIFE;
	_Feed = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0),VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	_Feed->SetColor(COLOR(0,0,0,0));
	_Timer = 0;

	_NariSclSpeed = NARI_SCL_SPEED;
	DriveSE = CSoundAL::Play(CSoundAL::SE_DRIVE,_Pos);
	DriveSE->SetVolume(0);
	IdlingSE = CSoundAL::Play(CSoundAL::SE_IDLING,_Pos);
 _DestPos = _Pos;
 _DestRot = _Rot;
 Barrel->SetDestPos(Barrel->Rot());
	_InputFlag = true;
 frame = 0;
 LaunchFlag = 0;
 MyId = MaxId;
 TargetId = -1;
 player[MyId] = this;
 DestRotY = 0;
 TargetSpeed = TargetPos = VECTOR3(0,0,0);
 Distance = 0;
 MaxId++;
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

	DriveSE->SetPos(_Pos);
	IdlingSE->SetPos(_Pos);

 //��������]
 _Rot = _DestRot;
 VECTOR3 diffRot;
 diffRot = (_DestRot - _Rot) * 0.2f;
 REVISE_PI_DEG(diffRot.x);
 REVISE_PI_DEG(diffRot.y);
 REVISE_PI_DEG(diffRot.z);
 AddRot(diffRot);
 REVISE_PI_DEG(_Rot.x);
 REVISE_PI_DEG(_Rot.y);
 REVISE_PI_DEG(_Rot.z);

 VECTOR3 posDiff;
 posDiff = ( _DestPos - _Pos ) * 0.2f;

 AddPos(posDiff);

	//���[�h�I��
	switch (_State)
	{
	case PLAYER_STATE_DEATH:
	{
  AddPosY(kUpSpeed + 0.0f);
  AddDestPosY(kUpSpeed + 0.0f);
		_Hegiht += kUpSpeed;
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;



		if (PlayerID == CManager::netData.charNum)
		{
			_Feed->AddAlpha(Alpha);
		}
  VECTOR3 pos = _Pos;
  pos.y += 10;
  pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
  pos.z += cosf(DEG2RAD(_Rot.y)) * -15;

  _nari->SetPos(pos);
		_nari->AddAlpha(Alpha * 3);
		Barrel->SetPos(_Pos);			// �ʒu
		CManager::SendPos(_Pos,PlayerID);
		CManager::SendRot(_Rot.y,PlayerID);

		CManager::SendCannonRot(Barrel->Rot(),PlayerID);


		if (_Hegiht > kHeightMax)
		{
			SetRespawn();
		}
		UpdateNari();
		return;
	}
	case PLAYER_STATE_RESPAWN:
	{
		float Alpha = (1.0f / kHeightMax) * kUpSpeed;
  AddPosY(-kUpSpeed);
  AddDestPosY(-kUpSpeed);
		_Hegiht += kUpSpeed;
  VECTOR3 pos = _Pos;
  pos.y += 20;
  pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
  pos.z += cosf(DEG2RAD(_Rot.y)) * -15;

  _nari->SetPos(pos);

		if (PlayerID == CManager::netData.charNum)
		{
			_Feed->AddAlpha(-Alpha);
		}
		_nari->AddAlpha(-Alpha * 3);
		Barrel->SetPos(_Pos);			// �ʒu
		CManager::SendPos(_Pos,PlayerID);
		CManager::SendRot(_Rot.y,PlayerID);
		CManager::SendCannonRot(Barrel->Rot(),PlayerID);

		if (_Hegiht > kHeightMax)
		{
			_State = PLAYER_STATE_WAIT;
			_Feed->SetAlpha(0);
			_nari->SetAlpha(0.0f);

			if(PlayerFlag)
			{
				Ballistic->SetDrawFlag(true);
			}

			CManager::SendReborn(PlayerID);
		}
		UpdateNari();
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
	// ���z�R���g���[���[
	VC* vc = VC::Instance();
	
	// �e�̎g�p��Ԃ��m��
	if (_Bullet == nullptr)
	{
		_BulletUseFlag = false;
	}

	// ������
	VECTOR3 rot = VECTOR3(0.0f,0.0f,0.0f);

	if (_InputFlag == true)
	{
		// �ړ�
		// ��

		Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed * vc->LeftStick().y;
		Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed * vc->LeftStick().y;
		
		if (vc->LeftStick().y > 0 || vc->LeftStick().y < 0)
		{
			IdlingSE->SetVolume(0);
			DriveSE->SetVolume(0.5f);
		}
		else
		{
			IdlingSE->SetVolume(0.5f);
			DriveSE->SetVolume(0);
		}

		//����
		rot.y -= PLAYER_ROTY_SPEED*vc->LeftStick().x;

		// �C�g�̏㉺
		BarrelRotX -= BARREL_ROTX_SPEED * vc->RightStick().y;

		// �Q�[�����I�����Ă��Ȃ���
		if(_EndGameFlag == false)
		{
			// �e�̔���
			// �e�����˂���Ă��Ȃ�������
			if (_LaunchFlag == false)
			{
				if (vc->Trigger(COMMAND_SHOT))
				{
					_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),_PlayerColor);
					vc->SetVibration(0.5f,20,0.5f,20);
					CSoundAL::Play(CSoundAL::SE_CANNON,_Pos);
					_LaunchFlag = true;
					_BulletUseFlag = true;
					_ReloadTimer = 0;
					CManager::SendCannon(_LaunchFlag,PlayerID);
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
					_LaunchFlag = false;
					_BulletUseFlag = false;
					_ReloadTimer = PLAYER_RELOAD_TIME;
				}
			}
		}
	}

	// �L�����N�^�[�̉�]
 AddRot(rot);
 SetDestRot(_Rot);

	// �l�̊ۂߍ���
	// �v���C���[�̉�]��

	REVISE_PI_DEG(_Rot.y);
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
 AddDestPos(Movement);

	// �ړ��G�t�F�N�g
	if (_SandTime >= 0)
	{
		_SandTime--;
	}
	else if (abs(Movement.x) > 0.1f || abs(Movement.z) > 0.1f)
	{
		VECTOR3	posEffect = _Pos;
		VECTOR3 vecMove = VECTOR3(0.0f,0.0f,0.0f);

		if (Movement.x != 0)
		{
			vecMove.x = Movement.x / abs(Movement.x);
		}

		if (Movement.z != 0)
		{
			vecMove.z = Movement.z / abs(Movement.z);
		}

		posEffect.x -= 8.0f * vecMove.x;
		posEffect.z -= 8.0f * vecMove.z;
		posEffect.y -= 10.0f;

		CSandCloud::Create(posEffect);
		_SandTime = 16;
	}

	// ����
	Movement *= 0.95f;

	// �C�g�̈ʒu�A��]���X�V
	Barrel->SetPos(_Pos);			// �ʒu
	Barrel->SetRot(_Rot);			// ��]
	Barrel->AddRotX(BarrelRotX);	// ��Őݒ肵����]�ʂɖC�g��X����]�ʂ����Z

	// �e���̍X�V
	Ballistic->Update(_Pos,VECTOR3(BarrelRotX,_Rot.y,_Rot.z));

	CManager::SendPos(_Pos,PlayerID);
	/*
	#ifdef ROT_QUART
	CManager::SendRot(VectorAxisRotation.x, VectorAxisRotation.y, VectorAxisRotation.z, RotationAxis, _Rot.y);
	#endif
	#ifdef ROT_NORMAL
	CManager::SendRot(_Rot.x, _Rot.y, _Rot.z, RotationAxis, rot.y);
	#endif
	*/
	CManager::SendRot(_Rot.y,PlayerID);
	CManager::SendCannonRot(Barrel->Rot(),PlayerID);

 CKeyboard* keyboard = CKeyboard::Instance();
 if(keyboard->GetPress(DIK_7))
 {
  _ReloadTimer = PLAYER_RELOAD_TIME;
 }

 // �f�o�b�O�p
	// �A��
	if (keyboard->GetPress(DIK_V))
	{
		CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),WHITE(0.5f));
	}

 // ���C�t�̉�
 if(keyboard->GetTrigger(DIK_B))
 {
  this->AddPlayerLife(1);
  _State = PLAYER_STATE_DAMAGE;
 }
 // ���C�t�̉�
	if (keyboard->GetTrigger(DIK_L))
	{
		this->AddPlayerLife(-1);
		_State = PLAYER_STATE_DAMAGE;
	}

	// �e�̍폜�m�F
	if (keyboard->GetTrigger(DIK_M))
	{
		this->ReleaseBullet();
}
}

//=============================================================================
//	�e���ˏ���
//=============================================================================
void CPlayer::BlastBullet()
{
	_Bullet = CBullet::Create(_Pos,VECTOR2(BULLET_SIZE,BULLET_SIZE),VECTOR3(BarrelRotX,_Rot.y,_Rot.z),_PlayerColor);
	CSoundAL::Play(CSoundAL::SE_CANNON,_Pos);
	_LaunchFlag = true;
	_BulletUseFlag = true;
	_ReloadTimer = 0;
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
 //��������]
 Barrel->SetRotY(Barrel->DestRot().y);
 Barrel->SetRotZ(Barrel->DestRot().z);
 VECTOR3 diffRot;
 VECTOR3 BarrelRot = Barrel->Rot();
 VECTOR3 BarrelDestRot = Barrel->DestRot();
 diffRot = (BarrelDestRot - BarrelRot)*0.2f;
 REVISE_PI_DEG(diffRot.x);
 Barrel->AddRot(diffRot);
 BarrelRot = Barrel->Rot();
 REVISE_PI_DEG(BarrelRot.x);
 Barrel->SetRot(BarrelRot);

	Barrel->SetPos(_Pos);			// �ʒu
	BarrelDestRotX = Barrel->Rot().x;

	// �ړ��G�t�F�N�g
	if (_SandTime >= 0)
	{
		_SandTime--;
	}
	else if (abs(Movement.x) > 0.1f || abs(Movement.z) > 0.1f)
	{
		VECTOR3	posEffect = _Pos;
		VECTOR3 vecMove = VECTOR3(0.0f,0.0f,0.0f);

		if (Movement.x != 0)
		{
			vecMove.x = Movement.x / abs(Movement.x);
		}

		if (Movement.z != 0)
		{
			vecMove.z = Movement.z / abs(Movement.z);
		}

		posEffect.x -= 8.0f * vecMove.x;
		posEffect.z -= 8.0f * vecMove.z;
		posEffect.y -= 10.0f;

		CSandCloud::Create(posEffect);
		_SandTime = 16;
	}

	if (abs(Movement.x) > 0.1f || abs(Movement.z) > 0.1f)
	{
		IdlingSE->SetVolume(0);
		DriveSE->SetVolume(0.3f);
	}
	else
	{
		IdlingSE->SetVolume(0.3f);
		DriveSE->SetVolume(0);
	}

	// �Q�[�����I�����Ă��Ȃ���
	if(_EndGameFlag == false)
	{
		// �e�����˂���Ă��鎞
		if (_LaunchFlag == true)
		{
			// �����[�h�\�܂ł̎��Ԃ����Z
			_ReloadTimer++;

			// �����[�h�������Ԃ𒴂�����
			if (_ReloadTimer >= PLAYER_RELOAD_TIME)
			{
				// �Ĕ��ˉ\��
				_LaunchFlag = false;
				_BulletUseFlag = false;
				_ReloadTimer = PLAYER_RELOAD_TIME;
			}
		}
	}

 if(CManager::CurrentScene != SCENE_GAME)
 {
  Movement = _Pos - _OldPos;
  return;
 }
 if(frame % 120 == 0)
 {
  if(TargetId < 0)
  {
   SurchTarget();

  }
  else
  {
   TargetId = -1;
  }
 }

 _Pos += Movement;
 _DestPos += Movement;

 MazzleRevision();
 if(TargetId >= 0)
 {
  VECTOR2 distance = VECTOR2(0,0);
  distance.x = TargetPos.x - _Pos.x;
  distance.y = TargetPos.z - _Pos.z;
  double dis = sqrt(distance.x*distance.x + distance.y*distance.y);
  if(dis < MIN_RANGE)
  {
   Movement.x -= sinf(DEG2RAD(_Rot.y)) * Speed;
   Movement.z -= cosf(DEG2RAD(_Rot.y)) * Speed;
  }
  else if(dis > MAX_RANGE*0.5f)
  {
   Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed;
   Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed;
  }
 }
 else
 {
  if(frame % 180 == 0)
  {
   DestRotY = rand() % 360 + .0f;
   REVISE_PI_DEG(DestRotY);

  }
  Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed;
  Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed;
  //SurchTarget();
 }

 frame++;
 if(frame > 60 * 30)
 {
  frame = 0;
 }

 Movement *= 0.95f;


 _ReloadTimer++;

 if(_ReloadTimer >= PLAYER_RELOAD_TIME)
 {
  LaunchFlag = true;
  _ReloadTimer = 0;
 }

 Shot();


 float SubRotY = DestRotY - _Rot.y;
 REVISE_PI_DEG(SubRotY);
 if(SubRotY < -TURN_SPEED)
 {
  SubRotY = -TURN_SPEED;
 }
 else if(SubRotY > TURN_SPEED)
 {
  SubRotY = TURN_SPEED;
 }
 _Rot.y += SubRotY;
 _DestRot.y += SubRotY;
 REVISE_PI_DEG(_Rot.y);
 REVISE_PI_DEG(_DestRot.y);

	// ���݂̍��W��ۑ�
	_OldPos = _Pos;
}
//------------------------------------------------------------------------------
// �Ȃ�̍X�V����
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CPlayer::UpdateNari(void)
{
	if (_State == PLAYER_STATE_RESPAWN || _State == PLAYER_STATE_DEATH)
	{
		if (_nari->Size().y > 1024 / NARI_SCL || _nari->Size().y < (1024 / NARI_SCL) / 2)
		{
			_NariSclSpeed *= -1;
		}
		_nari->AddSizeY(_NariSclSpeed);
	}
}
void CPlayer::setBarrelRot(VECTOR3 _rot)
{
	Barrel->SetRot(_rot);			// ��]
}
void CPlayer::setBarrelDestRot(VECTOR3 _rot)
{
 Barrel->SetDestRot(_rot);			// ��]
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
void CPlayer::SetDeath(VECTOR3 pos,int _charNum)
{
	if (_State != PLAYER_STATE_DEATH)
	{
		CSoundAL::Play(CSoundAL::SE_BREAK,_Pos);
		CSoundAL::Play(CSoundAL::SE_DEATH,_Pos);
		if (PlayerFlag)
		{
			VC::Instance()->SetVibration(1.0f,60,1.0f,180);
			Ballistic->SetDrawFlag(false);	// �e�����\����
		}
		_Hegiht = 0;
		_State = PLAYER_STATE_DEATH;
		_PlayerRespown = pos;
		_nari->SetAlpha(0.01f);

		if (_charNum == CManager::netData.charNum)
		{
			_Feed->SetAlpha(0);
			VECTOR3 pos = _Pos;
			pos.y += 10;
			pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
			pos.z += cosf(DEG2RAD(_Rot.y)) * -15;
			_nari->SetPos(pos);
		}
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
 SetDestPos(_PlayerRespown);
	_PlayerLife = PLAYER_LIFE;
	_ReloadTimer = PLAYER_RELOAD_TIME;

	if (PlayerID == CManager::netData.charNum)
	{
		_Feed->SetAlpha(1);
		_PlayerRespown.y += 10;

		VECTOR3 pos = _Pos;
		pos.y += 20;
		pos.x += sinf(DEG2RAD(_Rot.y)) * -15;
		pos.z += cosf(DEG2RAD(_Rot.y)) * -15;

		_nari->SetPos(pos);

		Movement = VECTOR3(0,0,0);

		Barrel->SetPos(_Pos);			// �ʒu
		CManager::SendPos(_Pos,PlayerID);

		/*
		#ifdef ROT_QUART
		CManager::SendRot(VectorAxisRotation.x, VectorAxisRotation.y, VectorAxisRotation.z, RotationAxis, _Rot.y);
		#endif

		#ifdef ROT_NORMAL
		CManager::SendRot(_Rot.x, _Rot.y, _Rot.z, RotationAxis, rot.y);
		#endif
		*/
		CManager::SendRot(_Rot.y,PlayerID);


		CManager::SendCannonRot(Barrel->Rot(),PlayerID);
		CManager::SendCannon(_LaunchFlag,PlayerID);

		// �t���O�̏�����
		_BulletUseFlag = false;
		_LaunchFlag = false;
	}
}
//------------------------------------------------------------------------------
// ���Z����
//------------------------------------------------------------------------------
// ����
// addVal	: ���C�t
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
		{
			if (_PlayerLife > PLAYER_LIFE)
			{
				_PlayerLife = PLAYER_LIFE;
			}
		}
	}
}

//------------------------------------------------------------------------------
//�G��T��
//------------------------------------------------------------------------------
void CPlayer::SurchTarget(void)
{
 double distance = DBL_MAX;
 double disBuff = DBL_MAX;
 int target = -1;
 VECTOR2 Sub = VECTOR2(0,0);
 for(int loop = 0;loop < PLAYER_MAX;loop++)
 {
  if(loop == MyId)
  {
   continue;
  }
  Sub.x = player[loop]->Pos().x - _Pos.x;
  Sub.y = player[loop]->Pos().z - _Pos.z;
  disBuff = sqrt(Sub.x*Sub.x + Sub.y*Sub.y);
  if(disBuff > SURCH_MAX)
  {
   continue;
  }
  if(disBuff < distance)
  {
   disBuff = distance;
   TargetPos = player[loop]->Pos();
   TargetSpeed = player[loop]->Movement;
   target = loop;
  }
 }
 TargetId = target;
}
//------------------------------------------------------------------------------
//�_��
//------------------------------------------------------------------------------
void CPlayer::Shot(void)
{
 if(TargetId == -1 || Distance > MAX_RANGE)
 {
  BarrelRotX = 0;
  return;
 }

 if(LaunchFlag == true)
 {
  LaunchFlag = false;
  cannon = true;
  BlastBullet();
 }
 else
 {
  cannon = false;
 }
}
//------------------------------------------------------------------------------
//�e���␳
//------------------------------------------------------------------------------
void CPlayer::MazzleRevision(void)
{
 if(TargetId >= 0)
 {

  VECTOR3 DestPos = TargetPos;
  VECTOR3 Sub = TargetPos - _Pos;
  VECTOR3 Dis = VECTOR3(0,0,0);
  float time = Sub.x / BULLET_SPEED;
  Distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

  DestPos += TargetSpeed * ( Distance / BULLET_SPEED );

  Sub = DestPos - _Pos;
  Distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

  Dis.x = BULLET_SPEED;

  //BarrelRotX = -asin((distance*GRAVITY) / (BULLET_SPEED*BULLET_SPEED))*0.5f;

  float per = Distance / MAX_RANGE;
  if(Distance > MAX_RANGE)
  {
   BarrelRotX = DEG2RAD(-10.0f);
  }
  else
  {
   BarrelRotX = DEG2RAD(MAXANGLE*per);
  }
  DestRotY = atan2(Sub.x,Sub.z);
  REVISE_PI(DestRotY);
  DestRotY = RAD2DEG(DestRotY);
 }
 REVISE_PI(BarrelRotX);
 Barrel->SetDestRot(_Rot);
 Barrel->SetRotY(_Rot.y);
 Barrel->SetRotZ(_Rot.z);
 Barrel->SetDestRotX(RAD2DEG(BarrelRotX));
}

//------------------------------------------------------------------------------
// EOF