//------------------------------------------------------------------------------
//
// �v���C���[ [CPlayer.h]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

#ifndef _C_PLAYER_H_
#define _C_PLAYER_H_

//------------------------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------------------------
#define PLAYER_MOVE_SPEED (0.05f)			// �ړ��ɂ�����W��
#define PLAYER_RELOAD_TIME (300)			// �����[�h�܂ł̃t���[��
#define PLAYER_LIFE (3)						// �v���C���[�̗͂̍ő�l
#define BARREL_ROT_MAX (-45.0f)				// �C�g�̍ő�p�x
#define BARREL_ROT_MIN (-10.0f)				// �C�g�̍ŏ��p�x
#define PLAYER_COLLISION_RADIUS (100.0f)	// �����蔻��p���̔��a

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include "Model.h"
#include "Polygon2D.h"
#include "Billboard.h"
#include "Ballistic.h"
//------------------------------------------------------------------------------
// �񋓑̐錾
//------------------------------------------------------------------------------
typedef enum
{
	PLAYER_STATE_WAIT = 0,	// ��~��
	PLAYER_STATE_MOVE,		// �ړ���
	PLAYER_STATE_DAMAGE,	// ��_���[�W
	PLAYER_STATE_DEATH,		// ���S
	PLAYER_STATE_RESPAWN,	// ������
	PLAYER_STATE_MAX,		// �ő吔�̑ޔ�
}PLAYER_STATE;

//------------------------------------------------------------------------------
// �O���錾
//------------------------------------------------------------------------------
class CBullet;
class CSoundAL;
//------------------------------------------------------------------------------
// �N���X�錾
//------------------------------------------------------------------------------

class CBallistic;

class CPlayer :public CModel
{
public:
	CPlayer(void);
	~CPlayer(void);

	void Init(void);
	void Update(void);

	// ���݂̑��U���ԃQ�b�^�[
	int ReloadTimer(void){ return _ReloadTimer; }

	// �̗�
	int PlayerLife(void){ return _PlayerLife; }				// �Q�b�^�[
	void SetPlayerLife(int life){ _PlayerLife = life; }		// �Z�b�^�[
	void AddPlayerLife(int addVal);	// ���Z

	// ����
	static CPlayer* Create(int modelID,const VECTOR3& pos,int PlayerID);

	// �e�|�C���^�Q�b�^�[
	CBullet* Bullet(void){ return _Bullet; }

	// ���
	PLAYER_STATE State(void){ return _State; }			// �Q�b�^�[
	void SetState(PLAYER_STATE state){ _State = state; }	// �Z�b�^�[

	void SetPlayerFlag(bool flag){ PlayerFlag = flag; }
	bool BulletUseFlag(void){ return _BulletUseFlag; }

	void ReleaseBullet(void);	// �e���폜

	void BlastBullet();
 void setBarrelRot(VECTOR3 _rot);
 void setBarrelDestRot(VECTOR3 _rot);

	void SetDeath(VECTOR3 posint,int _charNum);//���S����
	void SetRespawn(void);//��������

	void CreateBallistic(void){Ballistic = CBallistic::Create(COLOR(1.0f, 0.0f, 0.0f, 0.3f));}
	void SetBallisticDrawFlag(bool drawFlag){Ballistic->SetDrawFlag(drawFlag);}

	//	����
	//	�C���̃e�N�X�`���Z�b�g�֐�
	void setBarrelTex(int _texNum)
	{
		Barrel->SetTex(CTexture::Texture(_texNum));
	}
	//	�E���������Z
	void addKillCount()
	{
		killCount++;
	}
	int getKillCount()
	{
		return killCount;
	}
	//	�E���ꂽ�����Z
	void addDeathCount()
	{
		deathCount++;
	}
	int getDeathCount()
	{
		return deathCount;
	}
	//	�v���C���[�h�c�Q�b�^�[
	int getPlayerID()
	{
		return PlayerID;
	}

	void SetPlayerColor(COLOR color){_PlayerColor = color;}
	COLOR PlayerColor(void){return _PlayerColor;}

	void SetInputFlag(bool flag){_InputFlag = flag;}
	bool InputFlag(void){return _InputFlag;}
	bool GetPreyerFlag(void){ return PlayerFlag; }

	CBallistic* GetBallistic(void){return Ballistic;}

	bool LunchFlag(void){return _LaunchFlag;}
	void SetLaunchFlag(bool flag){_LaunchFlag = flag;}

	bool EndGameFlag(void){return _EndGameFlag;}
	void SetEndGameFlag(bool flag){_EndGameFlag = flag;}

private:
	void UpdatePlayer(void);	// �v���C���[���̍X�V
	void UpdateCPU(void);		// CPU���̍X�V
	void UpdateNari(void); //�Ȃ�X�V

	PLAYER_STATE _State;	// �v���C���[�̏��
	CModel* Barrel;			// �C�g
	CBullet* _Bullet;		// �e
	VECTOR3 Movement;		// �ړ���
	float Speed;			// �ړ����x
	float BarrelRotX;		// �C�g��X����]��
	bool _LaunchFlag;		// �e���˃t���O�itrue : �g�p / false : �s�g�p�j
	bool _BulletUseFlag;	// �e���݃t���O
	int _ReloadTimer;		// ���݂̑��U����
	int _PlayerLife;		// �̗�
	int PlayerID;			// �v���C���[���ʗpID
	bool PlayerFlag;		// ����L�����N�^�[���ǂ���
	CBallistic* Ballistic;	// �e��
	CPolygon2D* _Feed;
	int _Timer;				//���G���ԃ^�C�}�[
	float _Hegiht;			//�����ŃJ�E���g
	VECTOR3 _PlayerRespown;	//���̕����n�_�쐬
	CBillboard* _nari;
	int _SandTime;			// �y������
	VECTOR3 _OldPos;		// �O���W

	float _NariSclSpeed; // �Ȃ�̑傫���ς��X�s�[�h
	CSoundAL* DriveSE;
	CSoundAL* IdlingSE;

	int killCount;
	int deathCount;

	COLOR _PlayerColor;	// �v���C���[�̐F��ޔ�
	bool _InputFlag;	// ���͉s����itrue �\ / false �s�j

	bool _EndGameFlag;	// �Q�[���I���t���O[�I����̍s���}��] �itrue �I�� / false ��I���j
};

#endif

//------------------------------------------------------------------------------
// EOF