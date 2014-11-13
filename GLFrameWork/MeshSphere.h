//=============================================================================
//���̃N���X(�X�J�C�h�[��)
//=============================================================================
#ifndef _MESH_SPHERE_
#define _MESH_SPHERE_
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "Object.h"

//�N���X��`
class CMeshSphere:public CObject
{
public:
	CMeshSphere(int priority=0);
	~CMeshSphere();

	//=============================================================================
	//�쐬
	//�����F
	//	VECTOR3�F���S���W
	//	VECTOR2�F������
	//	float�F	���a
	//	VECTOR2�F�e�N�X�`���̃��s�[�g��
	//=============================================================================
	static CMeshSphere* Create(VECTOR3 pos,VECTOR2 PanelNum,float radius,VECTOR2 TexDivide=VECTOR2(3,1));

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	VECTOR2 PanelNum;
	VECTOR2 TexDiv;
	int IndexNum;
	int PolygonNum;
	int VertexNum;
	float Radius;

	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	int* Index;

};
#endif