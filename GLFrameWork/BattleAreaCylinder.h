//=============================================================================
//�V�����_�[�N���X
//=============================================================================
#ifndef _BATTLE_AREA_CYLINDER_
#define _BATTLE_AREA_CYLINDER_
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "MeshCylinder.h"

//�N���X��`
class CBattleAreaCylinder :public CMeshCylinder
{
public:
	CBattleAreaCylinder(int priority = 0);
	~CBattleAreaCylinder();

	//=============================================================================
	//�쐬
	//�����F
	//	VECTOR3:��̒��S
	//	float:	�|���S���ꖇ�̍���
	//	VECTOR2:������
	//	float:	���a
	//	VECTOR2:�e�N�X�`���̃��s�[�g��
	//=============================================================================
	static CBattleAreaCylinder* Create(VECTOR3 pos, float PanelHeight, VECTOR2 PanelNum, float radius, VECTOR2 TexDivide = VECTOR2(3, 1));

	void Init(void);
	void Draw(void);


private:
	COLOR*	Color;		// ���_�F

};
#endif