#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"
#include"UI.h"

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

class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
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

	static const int	MAX_ROCK;					// ��̐�

	void CheckHit(void);							// �U������̓����蔻��
	void PushBackCharacter(void);					// �L�����N�^�[���m�̉����߂�
	void PushBackRock(void);						// �L�����N�^�[�Ɗ�̉����߂�
	void PushBackField(void);						// �n�`�̉����߂�
	void PushBackObjectByField(CObject* pObject);	// �I�u�W�F�N�g�̒n�`�ɂ�鉟���߂�
	bool NeedsSkipPlayer(CPlayer* pPlayer);			// �v���C���[����X�L�b�v
	bool NeedsSkipBullet(CPlayer* pPlayer);			// �C�e����X�L�b�v

	CMeshGround*    Ground;     // �t�B�[���h
	CMeshSphere*    Sky;        // ��
	CPlayer*        Player;     // �v���C���[
	CPlayer*        Player2;    // �v���C���[
	//CReport*        reportBg;
	//CReport*        report;
	//CNumberManager* numberManager;
	CUI*		UI;

	CModel**		ppRock_;	//	��

};
#endif