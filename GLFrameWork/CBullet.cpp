//------------------------------------------------------------------------------
//
// �e [CBullet.cpp]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include <math.h>

#include "CBullet.h"
#include "CPlayer.h"

//------------------------------------------------------------------------------
// �R���X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CBullet::CBullet():CBillboard()
{
	// ���x
	Movement = VECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`��
	Texture = CTexture::Texture(TEX_TEST);
	SetUV(0,0,1.0f,1.0f);
}

//------------------------------------------------------------------------------
// �f�X�g���N�^
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
//------------------------------------------------------------------------------
CBullet::~CBullet()
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
void CBullet::Init(void)
{
	VECTOR3 rot;
	rot.x = DEG2RAD(_Rot.x);
	rot.y = DEG2RAD(_Rot.y);
	rot.z = 0;

	REVISE_PI(rot.x);
	REVISE_PI(rot.y);
	Movement.x = sinf(rot.y) * (cosf(rot.x)*BULLET_SPEED);
	Movement.y = -sinf(rot.x) * BULLET_SPEED;
	Movement.z = cosf(rot.y) * (cosf(rot.x)*BULLET_SPEED);

	CBillboard::Init();
}

//------------------------------------------------------------------------------
// �X�V
//------------------------------------------------------------------------------
// ����
//  �Ȃ�
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
void CBullet::Update(void)
{
	// �ړ�
	AddPos(Movement);

	// �����蔻��

	// ���x�̌���
	Movement.y -= 0.25f;

}

//------------------------------------------------------------------------------
// ����
//------------------------------------------------------------------------------
// ����
//  pos : �ʒu
//  size : �T�C�Y
//  rot : ��]
//  color : �F
// �߂�l
//  �Ȃ�
//------------------------------------------------------------------------------
CBullet* CBullet::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CBullet* Scene = new CBullet;
	Scene->_Pos = pos;
	Scene->_Rot = rot;
	Scene->_Size = size;
	Scene->_Color = color;

	Scene->Init();

	return Scene;
}

//------------------------------------------------------------------------------
// EOF