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
		rot.x -= 3.0f;
	}
	else if(CKeyboard::GetPress(DIK_DOWN))
	{
		rot.x += 3.0f;
	}

	// �L�����N�^�[�̉�]
	AddRot(rot);

	// �l�̊ۂߍ���
	if( Rot().y > 360.0f)
	{
		SetRotY(Rot().y - 2 * 360.0f);
	}
	else if (Rot().y < -360.0f)
	{
		SetRotY(Rot().y + 2 * 360.0f);
	}

	// �L�����N�^�[�̈ړ�
	AddPos(Movement);

	// ����
	Movement *= 0.95f;
 Barrel->SetPos(_Pos);

	// �U��
	//if(CKeyboard::GetTrigger(DIK_SPACE))
	//{
	//	if(Bullet == nullptr)
	//	{
	//		//LaunchFlag = true;
	//		//Bullet = CBullet::Create(_Pos, VECTOR2(40.0f, 40.0f), VECTOR3(0.0f, 0.0f, 0.0f), WHITE(0.5f));
	//		Bullet = CBullet::Create(_Pos, VECTOR2(BULLET_SIZE, BULLET_SIZE), _Rot, WHITE(0.5f));
	//	}
	//}

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
	Console::SetCursorPos(1,1);
	Console::Print("Rot.x:%f\nRot.y:%f\n",_Rot.x,_Rot.y);
#endif

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
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