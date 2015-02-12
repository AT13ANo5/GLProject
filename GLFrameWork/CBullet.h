//------------------------------------------------------------------------------
//
// �e [CBullet.h]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_


//------------------------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------------------------
#define BULLET_SIZE (10.0f)
#define BULLET_SPEED (3.5f)
#define BULLET_GRAVITY (-0.06f)	// �e�ɂ�����d��

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include "Billboard.h"

//------------------------------------------------------------------------------
// �N���X�錾
//------------------------------------------------------------------------------
class CBullet:public CBillboard
{
public:
	CBullet();
	CBullet(const CBullet& other);
	~CBullet();

	CBullet &operator=(const CBullet& other){ return *this; }

	void Init(void);
	void Update(void);

	static CBullet* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color);

private:
	VECTOR3 Movement;	// �ړ���
};

#endif

//------------------------------------------------------------------------------
// EOF