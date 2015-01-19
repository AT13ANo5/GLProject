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

//------------------------------------------------------------------------------
// �R���X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CPlayer::CPlayer():CModel()
{
	// �ړ����x
	Speed = PLAYER_MOVE_SPEED;

	// �e������
	Bullet = nullptr;

	// ���U����
	_ReloadTimer = PLAYER_RELOAD_TIME;

	// �C�g��X����]��
	BarrelRotX = 0.0f;
}

//------------------------------------------------------------------------------
// �f�X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CPlayer::~CPlayer()
{

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
	// �e
	Bullet = nullptr;

	// �p�����̏�����
	CModel::Init();

	// �C�g
	Barrel = CModel::Create(TANK_BARREL,_Pos);
	Barrel->Init();
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_RED));
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
	VECTOR3 rot = VECTOR3(0.0f, 0.0f, 0.0f);

	// �ړ�
	// ��
	if(CKeyboard::GetPress(DIK_W))
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
	if(CKeyboard::GetPress(DIK_A))
	{
		rot.y += 3.0f;
	}

	// �E
	else if(CKeyboard::GetPress(DIK_D))
	{
		rot.y -= 3.0f;
	}

	if(CKeyboard::GetPress(DIK_UP))
	{
		BarrelRotX -= 3.0f;
	}
	else if(CKeyboard::GetPress(DIK_DOWN))
	{
		BarrelRotX += 3.0f;
	}

	// �L�����N�^�[�̉�]
	AddRot(rot);

	// �l�̊ۂߍ���
	// �v���C���[�̉�]��
	if(Rot().y > 360.0f)
	{
		SetRotY(Rot().y - 2 * 360.0f);
	}
	else if(Rot().y < -360.0f)
	{
		SetRotY(Rot().y + 2 * 360.0f);
	}

	// �C�g
	if( BarrelRotX > 0.0f)
	{
		BarrelRotX = 0.0f;
	}
	else if(BarrelRotX < -45.0f)
	{
		BarrelRotX = -45.0f;
	}
	
	// �L�����N�^�[�̈ړ��l�����Z
	AddPos(Movement);

	// ����
	Movement *= 0.95f;

	// �C�g�̈ʒu�A��]���X�V
	Barrel->SetPos(_Pos);			// �ʒu
	Barrel->SetRot(_Rot);			// ��]
	Barrel->AddRotX(BarrelRotX);	// ��Őݒ肵����]�ʂɖC�g��X����]�ʂ����Z

	// �e�̔���
	if(LaunchFlag == false)
	{
		if(CKeyboard::GetTrigger(DIK_SPACE))
		{
			Bullet = CBullet::Create(_Pos, VECTOR2(BULLET_SIZE, BULLET_SIZE), _Rot, WHITE(0.5f));
			LaunchFlag = true;
			_ReloadTimer = 0;
		}
	}
	else
	{
		_ReloadTimer++;

		if(_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			LaunchFlag = false;
			_ReloadTimer = PLAYER_RELOAD_TIME;
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�p
	// �A��
	if(CKeyboard::GetPress(DIK_V))
	{
		Bullet = CBullet::Create(_Pos, VECTOR2(BULLET_SIZE, BULLET_SIZE), VECTOR3(BarrelRotX, _Rot.y, _Rot.z), WHITE(0.5f));
	}
#endif

#ifdef _DEBUG
	Console::SetCursorPos(1,1);
	Console::Print("Rot.x:%f\nRot.y:%f\n",_Rot.x,_Rot.y);
	Console::SetCursorPos(1, 3);
	Console::Print("barrel@rot.x : %f rot.y : %f rot.z : %f", Barrel->Rot().x, Barrel->Rot().y, Barrel->Rot().z);
#endif

}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
// ����
//  id : ���f����ID
//  pos : �����ʒu
// �߂�l
//  CPlayer* : ���������v���C���[�̃|�C���^
//------------------------------------------------------------------------------
CPlayer* CPlayer::Create(int id,const VECTOR3& pos)
{
	CPlayer* model = new CPlayer;
	if (model == nullptr){ return nullptr; }

	model->ModelID = id;
	model->_Pos = pos;
	model->Init();

	return model;
}

//------------------------------------------------------------------------------
// EOF