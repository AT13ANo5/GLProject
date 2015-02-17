//=============================================================================
// CSmoke�N���X [Smoke.h]
// Author :Mai Tanabe
//=============================================================================
#ifndef _MY_SMOKE_H
#define _MY_SMOKE_H
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "Billboard.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CSmoke : public CBillboard
{
public:
	CSmoke(){};
	~CSmoke(){};

	static void Create(const VECTOR3& pos);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_cntAnim;
};

#endif