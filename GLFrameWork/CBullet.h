//------------------------------------------------------------------------------
//
// 弾 [CBullet.h]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_
//------------------------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------------------------
#define BULLET_SIZE (40.0f)
#define BULLET_SPEED (10.0f)

//------------------------------------------------------------------------------
// ヘッダインクルード
//------------------------------------------------------------------------------
#include "Billboard.h"

//------------------------------------------------------------------------------
// クラス宣言
//------------------------------------------------------------------------------
class CBullet:public CBillboard
{
public:
	CBullet();
	~CBullet();

	void Init(void);
	void Update(void);

	static CBullet* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color);

private:
	VECTOR3 Movement;	// 移動量
};

#endif

//------------------------------------------------------------------------------
// EOF