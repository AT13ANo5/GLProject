//=============================================================================
// CExplosion�N���X [Explosion.h]
// Author :Mai Tanabe
//=============================================================================
#ifndef _MY_EXPLOSION_H
#define _MY_EXPLOSION_H
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "Billboard.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CExplosion : public CBillboard
{
public:
	CExplosion(){};
	~CExplosion(){};

	static void Create(const VECTOR3& pos);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_cntAnim;
};

#endif