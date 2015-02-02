#include "Model.h"

#include "Texture.h"
#include <Windows.h>
#include <stdio.h>

#define SCALE_MULTI (30.0f)

CModel* CModel::Top = nullptr;
CModel* CModel::Cur = nullptr;
int CModel::ModelNum = 0;
MODEL_DATA CModel::ModelData[CModel::MODEL_MAX];

const char* ModelFile[CModel::MODEL_MAX] = 
{
	"data/model/miku.mod",
	"data/model/tank_barrel.mod",
	"data/model/tank_youjo_normal.mod",
	"data/model/tank_youjo_rinchan.mod",
	"data/model/rock.mod",
};
//=============================================================================
//OpenGL�̃��f���N���X
//=============================================================================

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CModel::CModel(int priority):CObject(3)
{
	//�X�e�[�^�X������
	_Rot = VECTOR3(0,0,0);
	_Scl = VECTOR3(1.0f,1.0f,1.0f);

	FileName = nullptr;
	MotionPos = nullptr;
	MotionRot = nullptr;

	MotionFrameNum = 0;
	MotionID = 0;
	ModelNum++;
	LinkList();
}
//=============================================================================
//�f�X�g���N�^
//=============================================================================
//=============================================================================
//����
//����
//	int id:�g�p���郂�f���f�[�^��ID
//	VECTOR3 pos:�����ʒu
//=============================================================================
CModel* CModel::Create(int id,const VECTOR3& pos)
{
	CModel* model = new CModel;
	if (model == nullptr){ return nullptr; }

	model->ModelID = id;
	model->_Pos = pos;
	model->Init();

	return model;
}
//=============================================================================
//���g�����X�g�ɒǉ�
//=============================================================================
void CModel::LinkList(void)
{
	if(Top != nullptr)//��ڈȍ~�̏���
	{
		CModel* pScene = Cur;
		pScene->Next = this;
		Prev = pScene;
		Next = nullptr;
		Cur = this;
	}
	else//�ŏ��̈�̎��̏���
	{
		Top = this;
		Cur = this;
		Prev = nullptr;
		Next = nullptr;
	}
}

//=============================================================================
//���g�����X�g����폜
//=============================================================================
void CModel::UnlinkList(void)
{
	if(Prev == nullptr)//�擪
	{
		if(Next != nullptr)//��������
		{
			Next->Prev = nullptr;
			Top = Next;
		}
		else//�Ō�̈������
		{
			Top = nullptr;
			Cur = nullptr;
		}
	}
	else if(Next == nullptr)//�I�[
	{
		if(Prev != nullptr)//�O������
		{
			Prev->Next = nullptr;
			Cur = Prev;
		}
		else//�Ō�̈������
		{
			Top = nullptr;
			Cur = nullptr;
		}
	}
	else//�O��Ƀf�[�^������Ƃ�
	{
		Prev->Next = Next;
		Next->Prev = Prev;
	}

	Prev = nullptr;
	Next = nullptr;

	ModelNum--;
}
//=============================================================================
//����������
//=============================================================================
void CModel::Init(void)
{
	Material.ambient = COLOR(0.2f,0.2f,0.5f,1.0f);
	Material.diffuse = COLOR(1.0f,1.0f,1.0f,1.0f);
	Material.specular = COLOR(0,0,0,1.0f);
	Material.emission = COLOR(0,0,0,1.0f);
	Material.shininess = 0.0f;
}

void CModel::Initialize(void)
{
	for (int cnt = 0;cnt < MODEL_MAX;cnt++)
	{
		LoadModel(ModelFile[cnt],cnt);
	}
}

//=============================================================================
//�I������
//=============================================================================
void CModel::Uninit(void)
{
	if (MotionPos != nullptr)
	{
		for (int num = 0;num < ModelData[ModelID].PartsNum;num++)
		{
			if (MotionPos[num] != nullptr)
			{
				delete[] MotionPos[num];
			}
		}
		delete[] MotionPos;
	}
	if (MotionRot != nullptr)
	{
		for (int num = 0;num < ModelData[ModelID].PartsNum;num++)
		{
			if (MotionRot[num] != nullptr)
			{
				delete[] MotionRot[num];
			}
		}
		delete[] MotionRot;
	}
	//�e�N�X�`�������

	delete this;
}

void CModel::Finalize(void)
{
	for (int cnt = 0;cnt < MODEL_MAX;cnt++)
	{
		SafeDeletes(ModelData[cnt].Vtx);
		SafeDeletes(ModelData[cnt].Nor);
		SafeDeletes(ModelData[cnt].Tex);
		SafeDeletes(ModelData[cnt].TriIndexPos);
		SafeDeletes(ModelData[cnt].TriIndexNor);
		SafeDeletes(ModelData[cnt].TriIndexTex);
		SafeDeletes(ModelData[cnt].TriIndexNum);
		SafeDeletes(ModelData[cnt].QuadIndexPos);
		SafeDeletes(ModelData[cnt].QuadIndexNor);
		SafeDeletes(ModelData[cnt].QuadIndexTex);
		SafeDeletes(ModelData[cnt].QuadIndexNum);
	}
}
//=============================================================================
//�X�V
//=============================================================================
void CModel::Update(void)
{

}
//=============================================================================
//�`��
//=============================================================================
void CModel::Draw(void)
{
	//���C�e�B���O����
	glEnable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);//�J�����O����
	
	glMatrixMode(GL_MODELVIEW);
	int TriCount = 0;
	int QuadCount = 0;
	if (MotionFrameNum > 0)
	{
		MotionID++;
		MotionID = MotionID%MotionFrameNum;
	}
	for (int num = 0;num < ModelData[ModelID].PartsNum;num++)
	{
		glPushMatrix();//�r���[�}�g���b�N�X��ޔ�
		VECTOR3 pos(0,0,0),rot(0,0,0);
		//�`��ݒ�
		if (MotionPos != nullptr)
		{
			pos = MotionPos[num][MotionID];
			pos *= _Scl;
		}
		if (MotionRot != nullptr)
		{
			rot = MotionRot[num][MotionID];
			//rot /= 1.3f;
		}
		pos += _Pos;
		rot += _Rot;

		glTranslatef(pos.x,pos.y,pos.z);
		glScalef(1.0f,1.0f,1.0f);
		glRotatef(rot.y,0,1.0f,0);
		glRotatef(rot.x,1.0f,0,0);
		glRotatef(rot.z,0,0,1.0f);
		
		glBindTexture(GL_TEXTURE_2D,Texture.TexID);

		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);



		//�O�p�`�|���S���`��
		glBegin(GL_TRIANGLES);
		for (int cnt = 0;cnt < ModelData[ModelID].TriIndexNum[num] * 3;cnt++)
		{
			glColor4f(1.0f,1.0f,1.0f,1.0f);

			glNormal3f(ModelData[ModelID].Nor[ModelData[ModelID].TriIndexNor[TriCount] - 1].x,ModelData[ModelID].Nor[ModelData[ModelID].TriIndexNor[TriCount] - 1].y,ModelData[ModelID].Nor[ModelData[ModelID].TriIndexNor[TriCount] - 1].z);
			glTexCoord2f(ModelData[ModelID].Tex[ModelData[ModelID].TriIndexTex[TriCount] - 1].x,ModelData[ModelID].Tex[ModelData[ModelID].TriIndexTex[TriCount] - 1].y);
			glVertex3f(ModelData[ModelID].Vtx[ModelData[ModelID].TriIndexPos[TriCount] - 1].x*_Scl.x,ModelData[ModelID].Vtx[ModelData[ModelID].TriIndexPos[TriCount] - 1].y*_Scl.y,ModelData[ModelID].Vtx[ModelData[ModelID].TriIndexPos[TriCount] - 1].z*_Scl.z);
			TriCount++;
		}

		glEnd();

		//�l�p�`�|���S���`��
		glBegin(GL_QUADS);

		for (int cnt = 0; cnt < ModelData[ModelID].QuadIndexNum[num] * 4; cnt++)
		{
			glColor4f(1.0f,1.0f,1.0f,1.0f);

			glNormal3f(ModelData[ModelID].Nor[ModelData[ModelID].QuadIndexNor[QuadCount] - 1].x,ModelData[ModelID].Nor[ModelData[ModelID].QuadIndexNor[QuadCount] - 1].y,ModelData[ModelID].Nor[ModelData[ModelID].QuadIndexNor[QuadCount] - 1].z);
			glTexCoord2f(ModelData[ModelID].Tex[ModelData[ModelID].QuadIndexTex[QuadCount] - 1].x,ModelData[ModelID].Tex[ModelData[ModelID].QuadIndexTex[QuadCount] - 1].y);
			glVertex3f(ModelData[ModelID].Vtx[ModelData[ModelID].QuadIndexPos[QuadCount] - 1].x*_Scl.x,ModelData[ModelID].Vtx[ModelData[ModelID].QuadIndexPos[QuadCount] - 1].y*_Scl.y,ModelData[ModelID].Vtx[ModelData[ModelID].QuadIndexPos[QuadCount] - 1].z*_Scl.z);
		}
		glEnd();
		glPopMatrix();//�r���[�}�g���b�N�X��߂�
	}
	
	glEnable(GL_CULL_FACE);//�J�����O����
	
	glBindTexture(GL_TEXTURE_2D,0);
}
//=============================================================================
//���f���f�[�^�����[�h
//=============================================================================
bool CModel::LoadModel(const char* filename,int id)
{
	FILE* File = fopen(filename,"rb");
	if (File == nullptr){ return false; }

	unsigned short PosCount = 0,TexcoordCount = 0,NormalCount = 0;

	//���ꂼ��̑������擾
	fread(&ModelData[id].PartsNum,sizeof(unsigned short),1,File);
	ModelData[id].TriIndexNum = new int[ModelData[id].PartsNum];
	ModelData[id].QuadIndexNum = new int[ModelData[id].PartsNum];

	fread(&PosCount,sizeof(unsigned short),1,File);
	fread(&TexcoordCount,sizeof(unsigned short),1,File);
	fread(&NormalCount,sizeof(unsigned short),1,File);
	fread(ModelData[id].TriIndexNum,sizeof(int),ModelData[id].PartsNum,File);
	fread(ModelData[id].QuadIndexNum,sizeof(int),ModelData[id].PartsNum,File);

	//���_���
	ModelData[id].Vtx = new VECTOR3[PosCount];
	fread(ModelData[id].Vtx,sizeof(VECTOR3),PosCount,File);

	//�e�N�X�`�����W
	ModelData[id].Tex = new VECTOR2[TexcoordCount];
	fread(ModelData[id].Tex,sizeof(VECTOR2),TexcoordCount,File);

	//�@�����
	ModelData[id].Nor = new VECTOR3[NormalCount];
	fread(ModelData[id].Nor,sizeof(VECTOR3),NormalCount,File);

	//�O�p�`�C���f�b�N�X���
	int indexcount = ModelData[id].TriIndexSum();
	if (indexcount > 0)
	{
		ModelData[id].TriIndexPos = new int[indexcount * 3];
		ModelData[id].TriIndexTex = new int[indexcount * 3];
		ModelData[id].TriIndexNor = new int[indexcount * 3];

		for (int cnt = 0; cnt < indexcount * 3; cnt++)
		{
			fread(&ModelData[id].TriIndexPos[cnt],sizeof(int),1,File);
			fread(&ModelData[id].TriIndexTex[cnt],sizeof(int),1,File);
			fread(&ModelData[id].TriIndexNor[cnt],sizeof(int),1,File);
		}
	}
	indexcount = ModelData[id].QuadIndexSum();
	//�l�p�`�C���f�b�N�X���
	if (indexcount > 0)
	{
		ModelData[id].QuadIndexPos = new int[indexcount * 4];
		ModelData[id].QuadIndexTex = new int[indexcount * 4];
		ModelData[id].QuadIndexNor = new int[indexcount * 4];
		for (int cnt = 0; cnt < indexcount * 4; cnt++)
		{
			fread(&ModelData[id].QuadIndexPos[cnt],sizeof(int),1,File);
			fread(&ModelData[id].QuadIndexTex[cnt],sizeof(int),1,File);
			fread(&ModelData[id].QuadIndexNor[cnt],sizeof(int),1,File);
		}
	}

	fclose(File);
	Scaling(id,PosCount);

	return true;
}

CModel* CModel::GetModel(int id)
{
	CModel* Model = CModel::Top;
	int cnt=0;
	while(Model)
	{
		if(cnt==id)
		{
			return Model;
		}
		cnt++;
		Model = Model->CModel::Next;
	}

	return nullptr;
}

void CModel::Scaling(int id,int posCount)
{
	ModelData[id].Max = ModelData[id].Min = ModelData[id].Vtx[0];
	for (int cnt = 1;cnt<posCount;cnt++)
	{
		if (ModelData[id].Max.x < ModelData[id].Vtx[cnt].x)
		{
			ModelData[id].Max.x = ModelData[id].Vtx[cnt].x;
		}
		if (ModelData[id].Max.y < ModelData[id].Vtx[cnt].y)
		{
			ModelData[id].Max.y = ModelData[id].Vtx[cnt].y;
		}
		if (ModelData[id].Max.z < ModelData[id].Vtx[cnt].z)
		{
			ModelData[id].Max.z = ModelData[id].Vtx[cnt].z;
		}

		if (ModelData[id].Min.x > ModelData[id].Vtx[cnt].x)
		{
			ModelData[id].Min.x = ModelData[id].Vtx[cnt].x;
		}
		if (ModelData[id].Min.y > ModelData[id].Vtx[cnt].y)
		{
			ModelData[id].Min.y = ModelData[id].Vtx[cnt].y;
		}
		if (ModelData[id].Min.z > ModelData[id].Vtx[cnt].z)
		{
			ModelData[id].Min.z = ModelData[id].Vtx[cnt].z;
		}
	}

	ModelData[id].Size = ModelData[id].Max - ModelData[id].Min;
}

void CModel::LoadAnim(char* filename)
{
	if (MotionPos != nullptr)
	{
		for (int num = 0;num < ModelData[ModelID].PartsNum;num++)
		{
			if (MotionPos[num] != nullptr)
			{
				delete[] MotionPos[num];
			}
		}
		delete[] MotionPos;
	}
	if (MotionRot != nullptr)
	{
		for (int num = 0;num < ModelData[ModelID].PartsNum;num++)
		{
			if (MotionRot[num] != nullptr)
			{
				delete[] MotionRot[num];
			}
		}
		delete[] MotionRot;
	}
	FILE* Input = nullptr;
	
	fopen_s(&Input,filename,"rb");
	
	if (Input == nullptr)
	{
		return;
	}
	int frameNum = 0;
	fread(&frameNum,sizeof(int),1,Input);
	MotionFrameNum = frameNum;
	MotionPos = new VECTOR3*[ModelData[ModelID].PartsNum];
	MotionRot = new VECTOR3*[ModelData[ModelID].PartsNum];

	for (int cnt = 0;cnt < ModelData[ModelID].PartsNum;cnt++)
	{
		MotionPos[cnt] = new VECTOR3[frameNum];
		MotionRot[cnt] = new VECTOR3[frameNum];
	}
	for (int cnt = 0;cnt < frameNum;cnt++)
	{
		for (int num = 0;num < ModelData[ModelID].PartsNum;num++)
		{
			fread(&MotionPos[num][cnt],sizeof(VECTOR3),1,Input);
			fread(&MotionRot[num][cnt],sizeof(VECTOR3),1,Input);
		}
	}

	fclose(Input);
}