#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"
#include"UI.h"

class CBattleAreaCylinder;
class CMeshGround;
class CMeshSphere;
class CModel;
class CObject;
class CPlayer;
class CLoadGauge;
class CLoadString;
class CReport;
class CNumberManager;
class UI;
class CShadow;

class CGame :public CScene
{
public:

	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);

	typedef enum {
		PHASE_NONE = 0,
		PHASE_3,
		PHASE_2,
		PHASE_1,
		PHASE_START,
		PHASE_MAX
	} PHSE;


	static void SetPlayerState(NET_DATA _netData, DATA_TYPE _dataType);
	static void subTimer();


private:
	static const float  RADIUS_SKY;					// ��̔��a
	static const float	RADIUS_DEFENSE_CHARACTER;	// �L�����N�^�[�̖h�䔼�a
	static const float	HEIGHT_DEFENSE_CHARACTER;	// �L�����N�^�[�̖h�䒆�S����
	static const float	RADIUS_OFFENSE_BULLET;		// �C�e�̍U�����a
	static const float	HEIGHT_OFFENSE_BULLET;		// �C�e�̍U�����S����
	static const float	RADIUS_PUSH_CHARACTER;		// �L�����N�^�[�̉����߂����a
	static const float	HEIGHT_PUSH_CHARACTER;		// �L�����N�^�[�̉����߂����S����
	static const float	RADIUS_DEFENSE_ROCK;		// ��̖h�䔼�a
	static const float	HEIGHT_DEFENSE_ROCK;		// ��̖h�䒆�S����
	static const float	RADIUS_PUSH_ROCK;			// ��̉����߂����a
	static const float	HEIGHT_PUSH_ROCK;			// ��̉����߂����S����
	static const float FIELD_PANEL_SIZE;			//�t�B�[���h�̃p�l���ꖇ�̃T�C�Y
	static const float	HEIGHT_PLAYER_TO_FIELD;		// �v���C���[�ƒn�ʂ̍�

	static const float	RADIUS_AREA_BATTLE;			// �퓬�G���A���a
	static const float	HEIGHT_WALL;				// �ǂ̍���

	static const int	MAX_ROCK;					// ��̐�
	static const VECTOR3 ROCK_POSITION_LIST[];		// ��̍��W���X�g
 static const VECTOR3 PLAYER_POSITION_LIST[PLAYER_MAX]; // �v���C���[���W���X�g
 static const VECTOR3 PLAYER_ROTATION_LIST[PLAYER_MAX]; // �v���C���[�p�x���X�g

	void CheckHitPlayer(void);						// �U������̓����蔻�� �v���C���[
	void CheckHitRock(void);						// �U������̓����蔻�� ��
	void PushBackCharacter(void);					// �L�����N�^�[���m�̉����߂�
	void PushBackRock(void);						// �L�����N�^�[�Ɗ�̉����߂�
	void PushBackField(void);						// �n�`�̉����߂�
	void IsLandField(void);							// �n�`�Ƃ̔���
	void PushBackObjectByField(CObject* pObject, float offsetY);	// �I�u�W�F�N�g�̒n�`�ɂ�鉟���߂�
	void PushBackBattleArea(void);					// �s���\�͈͂̉����߂�
	bool NeedsSkipPlayer(CPlayer* pPlayer);			// �v���C���[����X�L�b�v
	bool NeedsSkipBullet(CPlayer* pPlayer);			// �C�e����X�L�b�v
	void HitBulletToField(void);						// ���e�n�_����
	void StartCount(void);

	CMeshGround*    Ground;     // �t�B�[���h
	CMeshSphere*    Sky;        // ��
	static CPlayer**       Player;     // �v���C���[	CNumberManager* numberManager;
	CBattleAreaCylinder*	CylinderArea;			// �퓬�\�G���A
	static CUI*		UI;
	CModel**		ppRock_;	//	��
	CShadow**		Shadow;		// �e

	PHSE				gamePhase;
	int					gamePhaseCnt;
};
#endif