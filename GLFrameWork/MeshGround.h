//=============================================================================
//�N���t���t�B�[���h�N���X
//=============================================================================
#ifndef _SCENE_GROUND_H_
#define _SCENE_GROUND_H_
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "main.h"
#include "Object.h"

typedef struct
{
	VECTOR2 uv;
	COLOR diffuse;
	VECTOR3 nor;
	VECTOR3 pos;

}VERTEX;

//�N���X��`
class CMeshGround :public CObject
{
public:
	CMeshGround(int priority = 0);
	~CMeshGround();
	//=============================================================================
	//�쐬
	//�����F
	//	VECTOR3�F���S���W
	//	VECTOR2�F�|���S���ꖇ�̃T�C�Y
	//	VECTOR2:������
	//=============================================================================
	static CMeshGround* Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum,float heightMag = 2.0f);

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=============================================================================
	//�������擾
	//=============================================================================
	float GetHeight(VECTOR3 pos,VECTOR3* normal = nullptr);

	//�Q�b�^�[
	VECTOR3 Size(void)const{ return _Size; }
	
private:

	VECTOR3 _Size;
	
	VECTOR2 PanelSize;
	VECTOR2 PanelNum;
	VECTOR2 ImgSize;
	int IndexNum;
	int PolygonNum;
	int VertexNum;
	float HeightMag;//�����̔{��
	GLuint DrawList;

	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	VECTOR3* NormalMap;
	float* HeightMap;
	int* Index;
	int MapNum;

	void GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY);
	float GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal = nullptr);
	void LoadImg(const char * imgFile);//�摜�f�[�^�ǂݍ���

};


#endif