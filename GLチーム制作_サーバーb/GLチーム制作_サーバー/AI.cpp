//=============================================================================
//	AI�N���X
//=============================================================================
//------------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------------
#include "AI.h"
#include<float.h>
#include <math.h>

//------------------------------------------------------------------------------
//�}�N����`
//------------------------------------------------------------------------------
// �p�֘A
#define PI     (3.141592654f) // �~����
#define RAD_TO_DEG   (57.29577951f) // 1���W�A�����x
#define DEG_TO_RAD   (0.017453293f) // 1�x�����W�A��

// �x�����W�A��
#define DEG2RAD(degree)  ((degree) * DEG_TO_RAD)
// ���W�A�����x
#define RAD2DEG(radian)  ((radian) * RAD_TO_DEG)

// �p�x�̐��K��(PI�`-PI)
#define REVISE_PI(radian) {if((radian) > PI){(radian) -= (2.0f * PI);}\
								else if((radian) < -PI){(radian) += (2.0f * PI);}}
//------------------------------------------------------------------------------
//�萔��`
//------------------------------------------------------------------------------
const float PLAYER_MOVE_SPEED (0.5f);			// �ړ��ɂ�����W��
const int UserMax = 6;//�ő僆�[�U�[��
const double SURCH_MAX = 3000.0f*3000.0f;	//�t�B�[���h�̃T�C�Y��1/4
const float GRAVITY = -0.08f;				//�e�ɂ�����d�͉����x
const float BULLET_SPEED = 10.0f;			//�e�̑��x
const float TURN_SPEED = DEG2RAD(3.0f);		//��]���x
const int PLAYER_RELOAD_TIME (300);			// �����[�h�܂ł̃t���[��
//------------------------------------------------------------------------------
//�ÓI�����o�ϐ���`
//------------------------------------------------------------------------------
AI* AI::Top = nullptr;
AI* AI::Cur = nullptr;

float leap(float start,float end,float delta)
{
	return (start*(1.0f-delta) + end*delta);
}
//------------------------------------------------------------------------------
//���X�g�ɒǉ�
//------------------------------------------------------------------------------
void AI::LinkList(void)
{
	if (Top != NULL)//��ڈȍ~�̏���
	{
		AI* ai = Cur;
		ai->Next = this;
		Prev = ai;
		Next = NULL;
		Cur = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top = this;
		Cur = this;
		Prev = NULL;
		Next = NULL;
	}
}

//------------------------------------------------------------------------------
//���X�g����폜
//------------------------------------------------------------------------------
void AI::UnlinkList(void)
{
	if (Prev == NULL)//�擪
	{
		if (Next != NULL)//��������
		{
			Next->Prev = NULL;
			Top = Next;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else if (Next == NULL)//�I�[
	{
		if (Prev != NULL)//�O������
		{
			Prev->Next = NULL;
			Cur = Prev;
		}
		else//�Ō�̈������
		{
			Top = NULL;
			Cur = NULL;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = NULL;
	Next = NULL;
}

//------------------------------------------------------------------------------
//�R���X�g���N�^
//------------------------------------------------------------------------------
AI::AI()
{
	UserInfo.cannon = false;
	UserInfo.entryFlag = false;
	UserInfo.death = 0;
	UserInfo.kill = 0;
	UserInfo.pos = VECTOR3(0,0,0);
	UserInfo.rot = VECTOR3(0,0,0);
	TargetPos = VECTOR3(0,0,0);
	TargetId = -1;
	DestRotY = 0;
	frame = 0;
	_ReloadTimer = 0;
	LinkList();
}
//------------------------------------------------------------------------------
//�f�X�g���N�^
//------------------------------------------------------------------------------
AI::~AI()
{
	UnlinkList();
}
//------------------------------------------------------------------------------
//������
//------------------------------------------------------------------------------
void AI::Initialize(void)
{
	for (int cnt = 0;cnt < UserMax;cnt++)
	{
		AI* ai = new AI;
		ai->ID = cnt;
	}
}
//------------------------------------------------------------------------------
//�I������
//------------------------------------------------------------------------------
void AI::Finalize(void)
{
	AI* ai = Top;
	AI* next = nullptr;
	while (ai)
	{
		next = ai->Next;
		delete ai;
		ai = nullptr;
		ai = next;
	}
}
//------------------------------------------------------------------------------
//�S�̍X�V
//------------------------------------------------------------------------------
void AI::UpdateAll(void)
{
	AI* ai = Top;
	
	//�S���̏�������

	while (ai)
	{
		ai->Update();
		ai = ai->Next;
	}

	//�S���̏��𑗂�
}
//------------------------------------------------------------------------------
//�X�V
//------------------------------------------------------------------------------
void AI::Update(void)
{
	if (UserInfo.entryFlag)
	{
		return ;
	}

	if (frame % 180 == 0)
	{
		SurchTarget();
	}

	UserInfo.pos += Movement;

	if (TargetId)
	{
		VECTOR2 distance = VECTOR2(0,0);
		distance.x = TargetPos.x - UserInfo.pos.x;
		distance.y = TargetPos.z - UserInfo.pos.z;
		DestRotY = atan2(distance.x,distance.y);
	}
	MazzleRevision();

	_ReloadTimer++;

	if (_ReloadTimer >= PLAYER_RELOAD_TIME)
	{
		LaunchFlag = false;
		_ReloadTimer = PLAYER_RELOAD_TIME;
	}
	
	Shot();

	float SubRotY = DestRotY-UserInfo.rot.y;
	REVISE_PI(SubRotY);
	if (SubRotY < -TURN_SPEED)
	{
		SubRotY = -TURN_SPEED;
	}
	else if (SubRotY > TURN_SPEED)
	{
		SubRotY = TURN_SPEED;
	}
	UserInfo.rot.y += SubRotY;

	frame++;
}

//------------------------------------------------------------------------------
//�G��T��
//------------------------------------------------------------------------------
void AI::SurchTarget(void)
{
	double distance = DBL_MAX;
	double disBuff = DBL_MAX;
	int target = -1;
	VECTOR2 Sub = VECTOR2(0,0);
	AI* ai = Top;
	while (ai)
	{
		if (ai->ID == ID)
		{
			continue;
		}
		Sub.x = ai->UserInfo.pos.x - UserInfo.pos.x;
		Sub.y = ai->UserInfo.pos.z - UserInfo.pos.z;
		disBuff = (Sub.x*Sub.x+Sub.y*Sub.y);
		if (disBuff > SURCH_MAX)
		{
			continue;
		}
		if (disBuff < distance)
		{
			disBuff = distance;
			target = ai->ID;
			TargetPos = ai->UserInfo.pos;
			TargetSpeed = ai->Movement;
		}
	}
	TargetId = target;
}
//------------------------------------------------------------------------------
//�_��
//------------------------------------------------------------------------------
void AI::Shot(void)
{
	if (TargetId == -1)
	{
		BarrelRotX = 0;
		return;
	}

	LaunchFlag = false;
	UserInfo.cannon = true;

	
}
//------------------------------------------------------------------------------
//���[�U�[�����Z�b�g
//------------------------------------------------------------------------------
void AI::SetUserInfo(USER_INFO* info)
{
	AI* ai = Top;
	int cnt = 0;
	while (ai)
	{
		ai->UserInfo = info[cnt];
		ai = ai->Next;
		cnt++;
	}
}
//------------------------------------------------------------------------------
//�e���␳
//------------------------------------------------------------------------------
void AI::MazzleRevision(void)
{
	if (TargetId >= 0)
	{
		VECTOR3 DestPos = TargetPos;
		VECTOR3 Sub = TargetPos - UserInfo.pos;
		VECTOR3 Dis = VECTOR3(0,0,0);
		float time = Sub.x / 20.0f;
		float distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

		DestPos += TargetSpeed * (distance / 20.0f);

		Sub = DestPos - UserInfo.pos;
		distance = sqrt(Sub.x*Sub.x + Sub.z*Sub.z);

		Dis.x = 20.0f;
		BarrelRotX = -asin((distance*0.25f) / (20.0f*20.0f))*0.5f;

		DestRotY = atan2(Sub.x,Sub.z);
	}
	REVISE_PI(BarrelRotX);
	REVISE_PI(DestRotY);

	//BallelRotX�𑗂�
}