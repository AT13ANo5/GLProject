#include "MeshGround.h"
#include "Texture.h"
#define SUM_INDEX(X,Z) ((X+1)*(Z-1)+((X+1)*(Z+1))+(Z-1)*2)

CMeshGround::CMeshGround(int priority) :CObject(priority)
{
	_Pos = VECTOR3(0,0,0);
	_Rot = VECTOR3(0,0,0);
	Vtx = nullptr;
	Tex = nullptr;
	Nor = nullptr;
	NormalMap = nullptr;
	HeightMap = nullptr;
}
CMeshGround::~CMeshGround()
{
	if (Vtx!=nullptr)
	{
		delete[] Vtx;
		Vtx = nullptr;
	}
	if (Tex!=nullptr)
	{
		delete[] Tex;
		Tex = nullptr;
	}
	if (Nor!=nullptr)
	{
		delete[] Nor;
		Nor = nullptr;
	}
	if (Index != nullptr)
	{
		delete[] Index;
		Index = nullptr;
	}
	if (NormalMap!=nullptr)
	{
		delete[] NormalMap;
		NormalMap = nullptr;
	}
	if (HeightMap!=nullptr)
	{
		delete[] HeightMap;
		HeightMap = nullptr;
	}
}
CMeshGround* CMeshGround::Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum)
{
	CMeshGround* Ground = new CMeshGround;
	if (Ground==nullptr)
	{
		return nullptr;
	}
	Ground->_Pos = pos;
	Ground->PanelSize = PanelSize;
	Ground->PanelNum = PanelNum;

	Ground->Init();

	return Ground;

}
void CMeshGround::Init(void)
{
	IndexNum = (int)(SUM_INDEX(PanelNum.x,PanelNum.y));
	VertexNum = (int)((PanelNum.x+1)*(PanelNum.y+1));
	PolygonNum = (int)(((PanelNum.x*2)*PanelNum.y)+((PanelNum.y-1)*4));
	MapNum = (int)(PanelNum.x*PanelNum.y*2);

	NormalMap = new VECTOR3[MapNum];
	HeightMap = new float[VertexNum];
	for (int cnt = 0;cnt<VertexNum;cnt++)
	{
		HeightMap[cnt] = rand()%60+0.0f;
	}

	Vtx = new VECTOR3[VertexNum];
	Tex = new VECTOR2[VertexNum];
	Nor = new VECTOR3[VertexNum];

	_Size.x = PanelNum.x*PanelSize.x;
	_Size.y = 0;
	_Size.z = PanelNum.y*PanelSize.y;

	float OffsetX = (PanelNum.x*PanelSize.x)/2;
	float OffsetZ = (PanelNum.y*PanelSize.y)/2;

	for (int LoopZ = 0,num = 0;LoopZ<PanelNum.y+1;LoopZ++)
	{
		for (int LoopX = 0;LoopX<PanelNum.x+1;LoopX++)
		{
			Vtx[num] = VECTOR3(OffsetX+(-PanelSize.x*LoopX),HeightMap[num],-OffsetZ+(PanelSize.y*LoopZ));
			Tex[num] = VECTOR2((float)LoopX,(float)LoopZ);
			Nor[num] = VECTOR3(0,1.0f,0);
			_Color = COLOR(1.0f,1.0f,1.0f,1.0f);
			num++;
		}
	}

	//�@���}�b�v���쐬
	VECTOR3 Vec1,Vec2,Cross;
	int cnt = 0;
	for (int Z = 0,num = 0;Z<PanelNum.y;Z++)
	{
		for (int X = 0;X<PanelNum.x;X++)
		{
			//�l�p�`���^�ŕ������ꂽ�㑤�̖@���̌v�Z
			Vec1 = Vtx[(int)(PanelNum.x+1)+num]-Vtx[num];
			Vec2 = Vtx[1+num]-Vtx[num];
			VECTOR3::Cross(&Cross,Vec2,Vec1);
			Cross.Normalize();
			NormalMap[cnt] = Cross;
			cnt++;

			//�l�p�`���^�ŕ������ꂽ�����̖@���̌v�Z
			Vec1 = Vtx[num+1]-Vtx[(int)(PanelNum.x+1)+(num+1)];
			Vec2 = Vtx[(int)(PanelNum.x+1)+num]-Vtx[(int)(PanelNum.x+1)+(num+1)];
			VECTOR3::Cross(&Cross,Vec2,Vec1);
			Cross.Normalize();
			NormalMap[cnt] = Cross;
			cnt++;
			num++;
		}
		//���_��X���̒[�܂ŗ����̂Ő܂�Ԃ�
		num++;
	}
	cnt = 0;

	//�@�����v�Z�����_�ɐݒ�
	for (int Y = 0,num = 0;Y<=PanelNum.y;Y++)
	{//num�͖ʂ̔ԍ�
		for (int X = 0;X<=PanelNum.x;X++)
		{
			VECTOR3 AddCross = VECTOR3(0,0,0);//�ꎞ�ϐ��������� �v�Z���ʊi�[�p
			if (X==0)
			{//X���̍��[�̎�
				if (Y==0)
				{//��ԍŏ��̒i�Ȃ�
					AddCross = NormalMap[num]+NormalMap[num+1];
				}
				else if (Y==PanelNum.y)
				{//��ԍŌ�̒i
					num = num-(int)PanelNum.x*2;//���O�̔ԍ��ɖ߂�
					AddCross = NormalMap[num];
					num--;//���������ԍ��Ȃ̂ň�߂��Ă���
				}
				else
				{//����
					AddCross = NormalMap[num-((int)PanelNum.x*2)]+NormalMap[num+1]+NormalMap[num];
				}
			}
			else
			{
				if (Y==0)
				{//��ԍŏ��̒i
					if (X==PanelNum.x)
					{//�E�[�̎�
						AddCross = NormalMap[num]+NormalMap[num+1];
					}
					else
					{
						AddCross = NormalMap[num]+NormalMap[num+1]+NormalMap[num+2];
					}
				}
				else if (Y==PanelNum.y)
				{//��ԍŌ�̒i
					if (X==PanelNum.x)
					{//�E�[�̎�
						AddCross = NormalMap[num]+NormalMap[num+1];
					}
					else
					{
						AddCross = NormalMap[num]+NormalMap[num+1]+NormalMap[num+2];
					}
				}
				else if (X==PanelNum.x)
				{//Y���[�ł͂Ȃ��A�����ɉE�[�������Ƃ�
					AddCross = NormalMap[num]+NormalMap[num-((int)PanelNum.x*2)]+NormalMap[num-((int)PanelNum.x*2)-1];
				}
				else
				{
					AddCross = NormalMap[num]+NormalMap[num+1]+NormalMap[num+2]+NormalMap[num-((int)PanelNum.x*2)-1]+NormalMap[num-((int)PanelNum.x*2)]+NormalMap[num-((int)PanelNum.x*2)+1];
				}
				if (X<PanelNum.x)
				{//�E�[�ȊO�Ȃ�ʂ̔ԍ�����i�߂�
					num++;
				}
			}

			//���K�������@���f�[�^�𒸓_�ɐݒ�
			AddCross.Normalize();
			Nor[cnt] = AddCross;
			cnt++;//���_�ԍ���i�߂�
			num++;//�ʂ̔ԍ���i�߂�
		}

	}

	int LoopX = 0;
	int VtxNo = 0;
	Index = new int[IndexNum];
	for (int LoopZ = 0;LoopZ<PanelNum.y;LoopZ++)
	{
		if (LoopZ!=0)
		{
			LoopX = 0;
			Index[VtxNo] = (int)((LoopZ*(PanelNum.x+1))+(((LoopX+1)%2)*(PanelNum.x+1)+(LoopX/2)));
			VtxNo++;
		}
		for (LoopX = 0;LoopX<(PanelNum.x+1)*2;LoopX++)
		{
			Index[VtxNo] = (int)((LoopZ*(PanelNum.x+1))+(((LoopX+1)%2)*(PanelNum.x+1)+(LoopX/2)));
			VtxNo++;
		}
		if (LoopZ==PanelNum.y-1)
		{
			break;
		}
		Index[VtxNo] = Index[VtxNo-1];
		VtxNo++;
	}
}

void CMeshGround::Uninit(void)
{
	delete this;
}
void CMeshGround::Update(void)
{
	//Rot.x++;

}
void CMeshGround::Draw(void)
{
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//�r���[�}�g���b�N�X��ޔ�
	//�`��ݒ�
	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glRotatef(_Rot.z,0,0,1.0f);
	glRotatef(_Rot.y,0,1.0f,0);
	glRotatef(_Rot.x,1.0f,0,0);
	glScalef(1.0f,1.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	//glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	//�|���S���`��
	glBegin(GL_TRIANGLE_STRIP);

	for (int cnt = 0;cnt<IndexNum;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[Index[cnt]].x,Nor[Index[cnt]].y,Nor[Index[cnt]].z);
		glTexCoord2f(Tex[Index[cnt]].x,Tex[Index[cnt]].y);
		glVertex3f(Vtx[Index[cnt]].x,Vtx[Index[cnt]].y,Vtx[Index[cnt]].z);
	}

	glEnd();

	glPopMatrix();//�r���[�}�g���b�N�X��߂�
	glBindTexture(GL_TEXTURE_2D,0);

}

void CMeshGround::GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY)
{
	pos.x += _Size.x/2;
	pos.z = _Size.z/2+(-pos.z);

	*OutIndexX = (int)(pos.x/PanelSize.x);
	*OutIndexY = (int)(pos.z/PanelSize.y);
}

float CMeshGround::GetHeight(VECTOR3 pos,VECTOR3* normal)
{
	int IndexX = 0;
	int IndexY = 0;
	GetPanelIndex(pos,&IndexX,&IndexY);
	int Index = (int)(IndexX+(IndexY*(PanelNum.x+1)));
	VECTOR3 VertexPos[4];
	VertexPos[0] = VECTOR3(IndexX*PanelSize.x,HeightMap[Index+((int)PanelNum.x+1)],(IndexY+1)*PanelSize.y);
	VertexPos[1] = VECTOR3(IndexX*PanelSize.x,HeightMap[Index],IndexY*PanelSize.y);
	VertexPos[2] = VECTOR3((IndexX+1)*PanelSize.x,HeightMap[Index+((int)PanelNum.x+2)],(IndexY+1)*PanelSize.y);
	VertexPos[3] = VECTOR3((IndexX+1)*PanelSize.x,HeightMap[Index+1],IndexY*PanelSize.y);
	for (int cnt = 0;cnt<4;cnt++)
	{
		VertexPos[cnt].x -= _Size.x/2;
		VertexPos[cnt].z -= _Size.z/2;
		VertexPos[cnt].z *= -1;
	}
	VECTOR3 Vec0 = VertexPos[1]-VertexPos[0];
	VECTOR3 Vec1 = VECTOR3(0,0,0);

	bool flag = false;
	for (int cnt = 0;cnt<3;cnt++)
	{
		Vec0 = VertexPos[((cnt+1)%3)]-VertexPos[(cnt%3)];
		Vec1 = pos-VertexPos[(cnt%3)];
		if ((Vec0.z*Vec1.x-Vec0.x*Vec1.z)>=0)
		{
			flag = true;
		}
		else
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{
		return GetHeightPolygon(VertexPos[0],VertexPos[1],VertexPos[2],pos);
	}
	Vec0 = VertexPos[3]-VertexPos[1];
	Vec1 = pos-VertexPos[1];
	if ((Vec0.z*Vec1.x-Vec0.x*Vec1.z)>=0)
	{
		Vec0 = VertexPos[2]-VertexPos[3];
		Vec1 = pos-VertexPos[3];
		if ((Vec0.z*Vec1.x-Vec0.x*Vec1.z)>=0)
		{
			Vec0 = VertexPos[1]-VertexPos[2];
			Vec1 = pos-VertexPos[2];
			if ((Vec0.z*Vec1.x-Vec0.x*Vec1.z)>=0)
			{
				return GetHeightPolygon(VertexPos[1],VertexPos[2],VertexPos[3],pos);
			}

		}

	}

	return 0;
}

float CMeshGround::GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal)
{
	VECTOR3 Vec1,Vec0;
	VECTOR3 normal = VECTOR3(0,0,0);
	Vec0 = p1-p0;
	Vec1 = p2-p0;
	VECTOR3::Cross(&normal,Vec0,Vec1);
	normal.Normalize();
	if (normal.y==0.0f)
	{
		return 0;
	}

	float Height = p0.y-(normal.x*(pos.x-p0.x)+normal.z*(pos.z-p0.z))/normal.y;

	return Height;

}