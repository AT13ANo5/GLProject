//=============================================================================
//�V�����_�[�N���X
//=============================================================================
#ifndef _MESH_CYLINDER_
#define _MESH_CYLINDER_
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Object.h"

//�N���X��`
class CMeshCylinder:public CObject
{
public:
	CMeshCylinder(int priority=0);
	~CMeshCylinder();

	//=============================================================================
	//�쐬
	//�����F
	//	VECTOR3:��̒��S
	//	float:	�|���S���ꖇ�̍���
	//	VECTOR2:������
	//	float:	���a
	//	VECTOR2:�e�N�X�`���̃��s�[�g��
	//=============================================================================
	static CMeshCylinder* Create(VECTOR3 pos,float PanelHeight,VECTOR2 PanelNum,float radius,VECTOR2 TexDivide=VECTOR2(3,1));

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:

	float PanelHeight;
	VECTOR2 PanelNum;
	VECTOR2 TexDiv;
	int IndexNum;
	int PolygonNum;
	int VertexNum;
	float Radius;
	GLuint DrawList;
	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	int* Index;

};
#endif