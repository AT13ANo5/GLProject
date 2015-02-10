//=============================================================================
// CExplosionクラス [Explosion.h]
// Author :Mai Tanabe
//=============================================================================
#ifndef _MY_EXPLOSION_H
#define _MY_EXPLOSION_H
//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "Billboard.h"

//=============================================================================
// クラス定義
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