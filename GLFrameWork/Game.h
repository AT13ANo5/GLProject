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
	static const float  RADIUS_SKY;		// ��̔��a

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