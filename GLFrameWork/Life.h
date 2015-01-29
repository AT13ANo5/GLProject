//=============================================================================
//
// 2D���C�t�N���X [Life.h]
//
// Auther : Kikushima
//          Masato Masuda
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CLife :public CPolygon2D
{
public:

  static const int LIFE_MAX = 3;

	CLife();
	~CLife(){}
	void Init(void);
	void Update(void);
  void Draw(void){}

	//=============================================================================
	//�쐬
	//�����F
	//	VECTOR3�F���S���W
	//	VECTOR2�F�傫��
	//	VECTOR3�F��]
	//	COLOR�F	���_�J���[
	//=============================================================================
	static CLife* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//=============================================================================
	//���C�t����
	//=============================================================================
  void LifeDamage(void){ LifePoint--; if (LifePoint < 0){ LifePoint = 0; } }	//�_���[�W
  void LifeReset(void){ LifePoint = LIFE_MAX; }	//����
	//=============================================================================
	// ���C�t�擾 �ݒ�
	//=============================================================================
	int GetLife(void){ return LifePoint; }//���݂̃��C�t�擾
  void SetLife(int life){ LifePoint = life; }

private:

  int LifePoint;//���݂̃��C�t
  CPolygon2D* Life[LIFE_MAX];

};

#endif

// end of file