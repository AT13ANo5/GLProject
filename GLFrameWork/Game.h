#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CMeshGround;
class CMeshSphere;
class CModel;
class CPlayer;
class CLoadGauge;
class CLoadString;
class CReport;
class CNumberManager;class CMiniMap;
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
	static const float	RADIUS_OFFENSE_BULLET;		// �C�e�̍U�����a
	static const float	RADIUS_PUSH_CHARACTER;		// �L�����N�^�[�̉����߂����a

	void CheckHit(void);				// �U������̓����蔻��
	void PushBackCharacter(void);		// �L�����N�^�[���m�̉����߂�
	void PushBackField(void);			// �n�`�̉����߂�

	CMeshGround*    Ground;     // �t�B�[���h
	CMeshSphere*    Sky;        // ��
	CPlayer*        Player;     // �v���C���[
	CLoadGauge*     loadGauge;  // ���U�Q�[�W
	CLoadString*    loadString; // ���U���̕���
	CReport*        reportBg;
	CReport*        report;
	CNumberManager* numberManager;
	CMiniMap*	MiniMap;

};
#endif