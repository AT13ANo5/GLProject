//=============================================================================
//	AI�N���X
//=============================================================================
#ifndef _AI_H_
#define _AI_H_

//------------------------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------------------------


#define BARREL_ROT_MAX (-60.0f)				// �C�g�̍ő�p�x
#define BARREL_ROT_MIN (0.0f)				// �C�g�̍ŏ��p�x

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include "main.h"

//------------------------------------------------------------------------------
// �N���X�錾
//------------------------------------------------------------------------------
class AI
{
public:
	AI();
	~AI();
	static void Initialize(void);
	static void Finalize(void);

	static void UpdateAll(void);
	
	// ���݂̑��U���ԃQ�b�^�[
	int ReloadTimer(void){ return _ReloadTimer; }

	// �̗̓Q�b�^�[
	int PlayerLife(void){ return _PlayerLife; }

	static void SetUserInfo(USER_INFO* info);
private:

	static AI* Top;
	static AI* Cur;
	AI* Next;
	AI* Prev;

	void LinkList(void);
	void UnlinkList(void);

	void Init(void);
	void Update(void);

	void SurchTarget(void);
	void Shot(void);
	void MazzleRevision(void);

	USER_INFO UserInfo;
	int TargetId;
	VECTOR3 TargetPos;
	VECTOR3 TargetSpeed;
	float DestRotY;
	int ID;

	VECTOR3 Movement;	// �ړ���
	float Speed;		// �ړ����x
	float BarrelRotX;	// �C�g��X����]��
	bool LaunchFlag;	// �e���˃t���O�itrue / �g�p�Ffalse / �s�g�p�j
	int _ReloadTimer;	// ���݂̑��U����
	int _PlayerLife;	// �̗�
	int frame;

};

#endif