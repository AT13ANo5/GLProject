//------------------------------------------------------------------------------
//
// �v���C���[ [CPlayer.h]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------------------------
#define PLAYER_MOVE_SPEED (0.5f)			// �ړ��ɂ�����W��
#define PLAYER_RELOAD_TIME (300)			// �����[�h�܂ł̃t���[��
#define PLAYER_LIFE (3)						// �v���C���[�̗͂̍ő�l
#define BARREL_ROT_MAX (-60.0f)				// �C�g�̍ő�p�x
#define BARREL_ROT_MIN (0.0f)				// �C�g�̍ŏ��p�x
#define PLAYER_COLLISION_RADIUS (100.0f)	// �����蔻��p���̔��a

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include "Model.h"

//------------------------------------------------------------------------------
// �O���錾
//------------------------------------------------------------------------------
class CBullet;

//------------------------------------------------------------------------------
// �N���X�錾
//------------------------------------------------------------------------------
class CPlayer:public CModel
{
public:
	CPlayer(void);
	~CPlayer(void);

	void Init(void);
	void Update(void);

	// ���݂̑��U���ԃQ�b�^�[
	int ReloadTimer(void){return _ReloadTimer;}

	// �̗̓Q�b�^�[
	int PlayerLife(void){return _PlayerLife;}

	static CPlayer* Create(int id, const VECTOR3& pos);

private:
	CModel* Barrel;		// �C�g
	CBullet* Bullet;	// �e
	VECTOR3 Movement;	// �ړ���
	float Speed;		// �ړ����x
	float BarrelRotX;	// �C�g��X����]��
	bool LaunchFlag;	// �e���˃t���O�itrue / �g�p�Ffalse / �s�g�p�j
	int _ReloadTimer;	// ���݂̑��U����
	int _PlayerLife;	// �̗�
};

//------------------------------------------------------------------------------
// EOF